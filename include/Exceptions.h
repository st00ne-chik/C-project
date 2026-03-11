#pragma once

#include <stdexcept>
#include <string>

class BookingException : public std::runtime_error {
public:
    explicit BookingException(const std::string& message)
        : std::runtime_error(message) {
    }
};

class EventNotFoundException final : public BookingException {
public:
    explicit EventNotFoundException(int eventId)
        : BookingException("Event with id " + std::to_string(eventId) + " was not found.") {
    }
};

class SeatAlreadyBookedException final : public BookingException {
public:
    SeatAlreadyBookedException(int eventId, int seatNumber)
        : BookingException("Seat " + std::to_string(seatNumber) +
                           " is already booked for event " + std::to_string(eventId) + ".") {
    }
};

class BookingNotFoundException final : public BookingException {
public:
    explicit BookingNotFoundException(int bookingId)
        : BookingException("Booking with id " + std::to_string(bookingId) + " was not found.") {
    }
};
