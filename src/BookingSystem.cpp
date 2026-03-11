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
