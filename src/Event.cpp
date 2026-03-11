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
