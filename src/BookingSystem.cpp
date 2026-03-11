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
