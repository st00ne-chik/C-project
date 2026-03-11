#pragma once

#include "Event.h"

#include <string>
#include <vector>

class Concert final : public Event {
public:
    Concert(int id,
            std::string title,
            std::string date,
            double price,
            std::string venueOrRoute,
            std::vector<Seat> seats,
            std::string performer,
            std::string genre);

    std::string getType() const override;
    std::string getDetails() const override;

    const std::string& getPerformer() const noexcept;
    const std::string& getGenre() const noexcept;

private:
    std::string performer_;
    std::string genre_;
};
