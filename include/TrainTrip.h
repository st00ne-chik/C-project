
#pragma once

#include "Event.h"

#include <string>
#include <vector>

class TrainTrip final : public Event {
public:
    TrainTrip(int id,
              std::string title,
              std::string date,
              double price,
              std::string venueOrRoute,
              std::vector<Seat> seats,
              std::string departureCity,
              std::string arrivalCity,
              std::string trainNumber);

    std::string getType() const override;
    std::string getDetails() const override;

    const std::string& getDepartureCity() const noexcept;
    const std::string& getArrivalCity() const noexcept;
    const std::string& getTrainNumber() const noexcept;

private:
    std::string departureCity_;
    std::string arrivalCity_;
    std::string trainNumber_;
};
