#pragma once

#include "Booking.h"
#include "Event.h"
#include "HistoryEntry.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class EventTypeFilter {
    All,
    Concert,
    MovieSession,
    TrainTrip
};

class BookingSystem {
public:
    static constexpr int MAX_MENU_ITEM = 7;

    void addEvent(std::unique_ptr<Event> event);
    void seedDemoData();

    const std::vector<std::unique_ptr<Event>>& getEvents() const noexcept;
    std::vector<const Event*> getEventsByType(EventTypeFilter filter) const;

    std::optional<std::reference_wrapper<const Event>> findEventById(int eventId) const;
    const Event& getEventByIdOrThrow(int eventId) const;

    Booking bookSeat(int eventId, int seatNumber, const std::string& customerName);
    void cancelBooking(int bookingId);

    const std::vector<Booking>& getBookings() const noexcept;
    std::optional<Booking> findBookingById(int bookingId) const;

    int countFreeSeats(int eventId) const;
    std::optional<int> findFirstFreeSeat(int eventId) const;

    const std::vector<HistoryEntry>& getHistory() const noexcept;

    static std::string toString(BookingStatus status);
    static std::string toString(HistoryActionType actionType);
    static std::string toString(EventTypeFilter filter);
    static std::string toString(const std::shared_ptr<const BookingHistoryContext>& context,
                                const HistoryPayload& payload);
private:
    std::vector<std::unique_ptr<Event>> events_;
    std::vector<Booking> bookings_;
    std::vector<HistoryEntry> history_;
    std::unordered_map<int, std::shared_ptr<const BookingHistoryContext>> bookingHistoryContexts_;
    int nextBookingId_{1};

    std::optional<std::size_t> findEventIndexById(int eventId) const;
    std::optional<std::size_t> findBookingIndexById(int bookingId) const;
    static std::string currentTimestamp();
    static bool isBlank(const std::string& value);
};
