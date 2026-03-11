#include "Event.h"

#include "Concert.h"
#include "MovieSession.h"
#include "TrainTrip.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

Event::Event(int id,
             std::string title,
             std::string date,
             double price,
             std::string venueOrRoute,
             std::vector<Seat> seats)
    : id_(id),
      title_(std::move(title)),
      date_(std::move(date)),
      price_(price),
      venueOrRoute_(std::move(venueOrRoute)),
      seats_(std::move(seats)) {
}

int Event::getId() const noexcept {
    return id_;
}

const std::string& Event::getTitle() const noexcept {
    return title_;
}

const std::string& Event::getDate() const noexcept {
    return date_;
}

double Event::getPrice() const noexcept {
    return price_;
}

const std::string& Event::getVenueOrRoute() const noexcept {
    return venueOrRoute_;
}

const std::vector<Seat>& Event::getSeats() const noexcept {
    return seats_;
}

bool Event::hasSeat(int seatNumber) const {
    return std::any_of(seats_.begin(), seats_.end(), [seatNumber](const Seat& seat) {
        return seat.seatNumber == seatNumber;
    });
}

bool Event::isSeatBooked(int seatNumber) const {
    for (const auto& seat : seats_) {
        if (seat.seatNumber == seatNumber) {
            return seat.isBooked;
        }
    }
    return false;
}

void Event::bookSeat(int seatNumber) {
    for (auto& seat : seats_) {
        if (seat.seatNumber == seatNumber) {
            seat.isBooked = true;
            return;
        }
    }
    throw std::invalid_argument("Seat does not exist.");
}

void Event::releaseSeat(int seatNumber) {
    for (auto& seat : seats_) {
        if (seat.seatNumber == seatNumber) {
            seat.isBooked = false;
            return;
        }
    }
    throw std::invalid_argument("Seat does not exist.");
}

int Event::freeSeatsCount() const {
    return static_cast<int>(std::count_if(seats_.begin(), seats_.end(), [](const Seat& seat) {
        return !seat.isBooked;
    }));
}

Concert::Concert(int id,
                 std::string title,
                 std::string date,
                 double price,
                 std::string venueOrRoute,
                 std::vector<Seat> seats,
                 std::string performer,
                 std::string genre)
    : Event(id, std::move(title), std::move(date), price, std::move(venueOrRoute), std::move(seats)),
      performer_(std::move(performer)),
      genre_(std::move(genre)) {
}

std::string Concert::getType() const {
    return "Concert";
}

std::string Concert::getDetails() const {
    std::ostringstream output;
    output << "Performer: " << performer_ << '\n'
           << "Genre: " << genre_;
    return output.str();
}

const std::string& Concert::getPerformer() const noexcept {
    return performer_;
}

const std::string& Concert::getGenre() const noexcept {
    return genre_;
}

MovieSession::MovieSession(int id,
                           std::string title,
                           std::string date,
                           double price,
                           std::string venueOrRoute,
                           std::vector<Seat> seats,
                           std::string movieRating,
                           int durationMinutes)
    : Event(id, std::move(title), std::move(date), price, std::move(venueOrRoute), std::move(seats)),
      movieRating_(std::move(movieRating)),
      durationMinutes_(durationMinutes) {
}
