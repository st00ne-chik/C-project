#pragma once

#include <string>

enum class BookingStatus {
    Active,
    Canceled
};

struct Booking {
    int bookingId{};
    int eventId{};
    int seatNumber{};
    std::string customerName;
    BookingStatus status{BookingStatus::Active};
    std::string timestamp;
};
