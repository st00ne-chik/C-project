#include "BookingSystem.h"

#include "Concert.h"
#include "Exceptions.h"
#include "MovieSession.h"
#include "TrainTrip.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

namespace {

std::vector<Seat> makeSeats(int count) {
    std::vector<Seat> seats;
    seats.reserve(count);
    for (int seatNumber = 1; seatNumber <= count; ++seatNumber) {
        seats.push_back(Seat{seatNumber, false});
    }
    return seats;
}

}  // namespace

void BookingSystem::addEvent(std::unique_ptr<Event> event) {
    if (!event) {
        throw BookingException("Cannot add an empty event.");
    }

    if (findEventIndexById(event->getId()).has_value()) {
        throw BookingException("Event with this id already exists.");
    }

    events_.push_back(std::move(event));
}
void BookingSystem::seedDemoData() {
    events_.clear();
    bookings_.clear();
    history_.clear();
    bookingHistoryContexts_.clear();
    nextBookingId_ = 1;

    addEvent(std::make_unique<Concert>(1,
                                       "Rock Night",
                                       "2026-04-20",
                                       2300.0,
                                       "Moscow Arena",
                                       makeSeats(8),
                                       "Neon Waves",
                                       "Alternative Rock"));

    addEvent(std::make_unique<Concert>(2,
                                       "Symphony Evening",
                                       "2026-05-02",
                                       1800.0,
                                       "Grand Hall",
                                       makeSeats(6),
                                       "City Orchestra",
                                       "Classical"));

    addEvent(std::make_unique<MovieSession>(3,
                                            "Interstellar: Re-Run",
                                            "2026-03-29",
                                            600.0,
                                            "Cinema Hall #2",
                                            makeSeats(7),
                                            "PG-13",
                                            169));

    addEvent(std::make_unique<MovieSession>(4,
                                            "The Grand Budapest Hotel",
                                            "2026-03-30",
                                            550.0,
                                            "Cinema Hall #1",
                                            makeSeats(9),
                                            "16+",
                                            100));

    addEvent(std::make_unique<TrainTrip>(5,
                                         "Moscow -> Saint Petersburg",
                                         "2026-04-05",
                                         4200.0,
                                         "Moscow - Saint Petersburg",
                                         makeSeats(10),
                                         "Moscow",
                                         "Saint Petersburg",
                                         "Sapsan-752A"));

    addEvent(std::make_unique<TrainTrip>(6,
                                         "Kazan -> Sochi",
                                         "2026-04-09",
                                         3900.0,
                                         "Kazan - Sochi",
                                         makeSeats(8),
                                         "Kazan",
                                         "Sochi",
                                         "Volga-128"));
}


const std::vector<std::unique_ptr<Event>>& BookingSystem::getEvents() const noexcept {
    return events_;
}

std::vector<const Event*> BookingSystem::getEventsByType(EventTypeFilter filter) const {
    std::vector<const Event*> result;
    result.reserve(events_.size());

    const std::string expectedType = toString(filter);
    for (const auto& event : events_) {
        if (filter == EventTypeFilter::All || event->getType() == expectedType) {
            result.push_back(event.get());
        }
    }
    return result;
}


std::optional<std::reference_wrapper<const Event>> BookingSystem::findEventById(int eventId) const {
    const auto eventIndex = findEventIndexById(eventId);
    if (!eventIndex.has_value()) {
        return std::nullopt;
    }
    return std::cref(*events_.at(*eventIndex));
}

const Event& BookingSystem::getEventByIdOrThrow(int eventId) const {
    const auto event = findEventById(eventId);
    if (!event.has_value()) {
        throw EventNotFoundException(eventId);
    }
    return event->get();
}

Booking BookingSystem::bookSeat(int eventId, int seatNumber, const std::string& customerName) {
    if (isBlank(customerName)) {
        throw BookingException("Customer name cannot be empty.");
    }

    const auto eventIndex = findEventIndexById(eventId);
    if (!eventIndex.has_value()) {
        throw EventNotFoundException(eventId);
    }

    Event& event = *events_.at(*eventIndex);
    if (!event.hasSeat(seatNumber)) {
        throw BookingException("Seat " + std::to_string(seatNumber) + " does not exist.");
    }

    if (event.isSeatBooked(seatNumber)) {
        throw SeatAlreadyBookedException(eventId, seatNumber);
    }

    event.bookSeat(seatNumber);

    Booking booking;
    booking.bookingId = nextBookingId_++;
    booking.eventId = eventId;
    booking.seatNumber = seatNumber;
    booking.customerName = customerName;
    booking.status = BookingStatus::Active;
    booking.timestamp = currentTimestamp();

    bookings_.push_back(booking);

    const auto context = std::make_shared<BookingHistoryContext>(
        BookingHistoryContext{booking.bookingId, booking.eventId, booking.seatNumber, booking.customerName});
    bookingHistoryContexts_[booking.bookingId] = context;
    const HistoryPayload createdPayload = BookingCreatedPayload{};

    history_.push_back(HistoryEntry{
        HistoryActionType::Created,
        booking.bookingId,
        context,
        createdPayload,
        toString(context, createdPayload),
        currentTimestamp()});

    return booking;
}

void BookingSystem::cancelBooking(int bookingId) {
    const auto bookingIndex = findBookingIndexById(bookingId);
    if (!bookingIndex.has_value()) {
        throw BookingNotFoundException(bookingId);
    }

    Booking& booking = bookings_.at(*bookingIndex);
    if (booking.status == BookingStatus::Canceled) {
        throw BookingException("Booking with id " + std::to_string(bookingId) + " is already canceled.");
    }

    const auto eventIndex = findEventIndexById(booking.eventId);
    if (!eventIndex.has_value()) {
        throw EventNotFoundException(booking.eventId);
    }

    Event& event = *events_.at(*eventIndex);
    event.releaseSeat(booking.seatNumber);
    booking.status = BookingStatus::Canceled;

    auto contextIterator = bookingHistoryContexts_.find(booking.bookingId);
    if (contextIterator == bookingHistoryContexts_.end()) {
        const auto fallbackContext = std::make_shared<BookingHistoryContext>(BookingHistoryContext{
            booking.bookingId, booking.eventId, booking.seatNumber, booking.customerName});
        contextIterator = bookingHistoryContexts_.emplace(booking.bookingId, std::move(fallbackContext)).first;
    }
    const auto& context = contextIterator->second;
    const HistoryPayload canceledPayload = BookingCanceledPayload{"Canceled by user request."};

    history_.push_back(HistoryEntry{
        HistoryActionType::Canceled,
        booking.bookingId,
        context,
        canceledPayload,
        toString(context, canceledPayload),
        currentTimestamp()});
}

const std::vector<Booking>& BookingSystem::getBookings() const noexcept {
    return bookings_;
}

std::optional<Booking> BookingSystem::findBookingById(int bookingId) const {
    const auto bookingIndex = findBookingIndexById(bookingId);
    if (!bookingIndex.has_value()) {
        return std::nullopt;
    }
    return bookings_.at(*bookingIndex);
}

int BookingSystem::countFreeSeats(int eventId) const {
    return getEventByIdOrThrow(eventId).freeSeatsCount();
}

std::optional<int> BookingSystem::findFirstFreeSeat(int eventId) const {
    const Event& event = getEventByIdOrThrow(eventId);
    for (const auto& seat : event.getSeats()) {
        if (!seat.isBooked) {
            return seat.seatNumber;
        }
    }
    return std::nullopt;
}

const std::vector<HistoryEntry>& BookingSystem::getHistory() const noexcept {
    return history_;
}

std::string BookingSystem::toString(BookingStatus status) {
    switch (status) {
        case BookingStatus::Active:
            return "Active";
        case BookingStatus::Canceled:
            return "Canceled";
    }
    return "Unknown";
}
