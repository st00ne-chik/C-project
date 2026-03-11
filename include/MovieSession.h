
#pragma once

#include "Event.h"

#include <string>
#include <vector>

class MovieSession final : public Event {
public:
    MovieSession(int id,
                 std::string title,
                 std::string date,
                 double price,
                 std::string venueOrRoute,
                 std::vector<Seat> seats,
                 std::string movieRating,
                 int durationMinutes);

    std::string getType() const override;
    std::string getDetails() const override;

    const std::string& getMovieRating() const noexcept;
    int getDurationMinutes() const noexcept;
