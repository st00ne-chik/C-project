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
