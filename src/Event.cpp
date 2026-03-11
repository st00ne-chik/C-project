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
