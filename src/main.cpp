#include "BookingSystem.h"
#include "Exceptions.h"

#include <clocale>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

constexpr int MAX_MENU_ITEM = BookingSystem::MAX_MENU_ITEM;
constexpr const char* SEPARATOR = "------------------------------------------------------------";

void setupConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::setlocale(LC_ALL, "");
}

void printHeader(const std::string& title) {
    std::cout << '\n' << SEPARATOR << '\n';
    std::cout << title << '\n';
    std::cout << SEPARATOR << '\n';
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value{};
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

void showEvents(const BookingSystem& system, EventTypeFilter filter = EventTypeFilter::All) {
    printHeader("Available events");
    const auto events = system.getEventsByType(filter);

    if (events.empty()) {
        std::cout << "No events found.\n";
        return;
    }

    std::cout << std::left << std::setw(4) << "ID" << std::setw(14) << "Type" << std::setw(28) << "Title"
              << std::setw(13) << "Date" << std::setw(24) << "Venue/Route" << std::setw(10) << "Price"
              << std::setw(6) << "Free" << '\n';
    std::cout << SEPARATOR << '\n';

    std::cout << std::fixed << std::setprecision(2);
    for (const auto* event : events) {
        std::cout << std::left << std::setw(4) << event->getId() << std::setw(14) << event->getType().substr(0, 13)
                  << std::setw(28) << event->getTitle().substr(0, 27) << std::setw(13) << event->getDate()
                  << std::setw(24) << event->getVenueOrRoute().substr(0, 23) << std::setw(10) << event->getPrice()
                  << std::setw(6) << event->freeSeatsCount() << '\n';
    }
}

void showEventDetails(const BookingSystem& system) {
    const int eventId = readInt("Enter event id: ");
    const auto event = system.findEventById(eventId);
    if (!event.has_value()) {
        std::cout << "Event with this id was not found.\n";
        return;
    }

    const Event& value = event->get();
    printHeader("Event details");
    std::cout << "ID: " << value.getId() << '\n';
    std::cout << "Type: " << value.getType() << '\n';
    std::cout << "Title: " << value.getTitle() << '\n';
    std::cout << "Date: " << value.getDate() << '\n';
    std::cout << "Venue/Route: " << value.getVenueOrRoute() << '\n';
    std::cout << "Price: " << value.getPrice() << '\n';
    std::cout << value.getDetails() << '\n';

    std::cout << "Seats: ";
    for (const auto& seat : value.getSeats()) {
        std::cout << seat.seatNumber << (seat.isBooked ? "[X] " : "[ ] ");
    }
    std::cout << '\n';
}

void bookTicket(BookingSystem& system) {
    printHeader("Create booking");
    const int eventId = readInt("Enter event id: ");
    const int seatNumber = readInt("Enter seat number: ");
    const std::string customerName = readLine("Enter customer name: ");

    const Booking booking = system.bookSeat(eventId, seatNumber, customerName);
    std::cout << "Booking created successfully.\n";
    std::cout << "Booking ID: " << booking.bookingId << '\n';
    std::cout << "Timestamp: " << booking.timestamp << '\n';
}

void cancelBooking(BookingSystem& system) {
    printHeader("Cancel booking");
    const int bookingId = readInt("Enter booking id: ");
    system.cancelBooking(bookingId);
    std::cout << "Booking " << bookingId << " canceled.\n";
}

void showBookings(const BookingSystem& system) {
    printHeader("All bookings");
    const auto& bookings = system.getBookings();
    if (bookings.empty()) {
        std::cout << "No bookings yet.\n";
        return;
    }

    std::cout << std::left << std::setw(8) << "ID" << std::setw(8) << "Event" << std::setw(8) << "Seat"
              << std::setw(20) << "Customer" << std::setw(10) << "Status" << "Timestamp\n";
    std::cout << SEPARATOR << '\n';

    for (const auto& booking : bookings) {
        std::cout << std::left << std::setw(8) << booking.bookingId << std::setw(8) << booking.eventId
                  << std::setw(8) << booking.seatNumber << std::setw(20) << booking.customerName.substr(0, 19)
                  << std::setw(10) << BookingSystem::toString(booking.status) << booking.timestamp << '\n';
    }
}

void showHistory(const BookingSystem& system) {
    printHeader("Booking history");
    const auto& history = system.getHistory();
    if (history.empty()) {
        std::cout << "History is empty.\n";
        return;
    }

    std::cout << std::left << std::setw(12) << "Action" << std::setw(10) << "BookingID" << std::setw(22) << "Timestamp"
              << "Message\n";
    std::cout << SEPARATOR << '\n';

    for (const auto& entry : history) {
        std::cout << std::left << std::setw(12) << BookingSystem::toString(entry.actionType) << std::setw(10)
                  << entry.bookingId << std::setw(22) << entry.timestamp << entry.message << '\n';
    }
}

void filterByType(const BookingSystem& system) {
    printHeader("Filter by type");
    std::cout << "1. Concert\n";
    std::cout << "2. MovieSession\n";
    std::cout << "3. TrainTrip\n";
    const int option = readInt("Choose type: ");

    switch (option) {
        case 1:
            showEvents(system, EventTypeFilter::Concert);
            break;
        case 2:
            showEvents(system, EventTypeFilter::MovieSession);
            break;
        case 3:
            showEvents(system, EventTypeFilter::TrainTrip);
            break;
        default:
            std::cout << "Unknown filter type.\n";
            break;
    }
}

void showMenu() {
    printHeader("Ticket Booking Console System");
    std::cout << "1. Show all events\n";
    std::cout << "2. Show event details\n";
    std::cout << "3. Book ticket\n";
    std::cout << "4. Cancel booking\n";
    std::cout << "5. Show all bookings\n";
    std::cout << "6. Show booking history\n";
    std::cout << "7. Filter by type\n";
    std::cout << "0. Exit\n";
}

}
