#pragma once

#include <memory>
#include <string>
#include <variant>

enum class HistoryActionType {
    Created,
    Canceled
};

struct BookingHistoryContext {
    int bookingId{};
    int eventId{};
    int seatNumber{};
    std::string customerName;
};

struct BookingCreatedPayload {
};

struct BookingCanceledPayload {
    std::string reason;
};

using HistoryPayload = std::variant<BookingCreatedPayload, BookingCanceledPayload>;

struct HistoryEntry {
    HistoryActionType actionType{HistoryActionType::Created};
    int bookingId{};
    std::shared_ptr<const BookingHistoryContext> context;
    HistoryPayload payload{BookingCreatedPayload{}};
    std::string message;
    std::string timestamp;
};
