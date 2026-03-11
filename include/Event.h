#pragma once

#include "Seat.h"

#include <string>
#include <vector>

class Event {
public:
    Event(int id,
          std::string title,
          std::string date,
          double price,
          std::string venueOrRoute,
          std::vector<Seat> seats);
    virtual ~Event() = default;

    int getId() const noexcept;
    const std::string& getTitle() const noexcept;
    const std::string& getDate() const noexcept;
    double getPrice() const noexcept;
    const std::string& getVenueOrRoute() const noexcept;
    const std::vector<Seat>& getSeats() const noexcept;

    bool hasSeat(int seatNumber) const;
    bool isSeatBooked(int seatNumber) const;
    void bookSeat(int seatNumber);
    void releaseSeat(int seatNumber);
    int freeSeatsCount() const;

    virtual std::string getType() const = 0;
    virtual std::string getDetails() const = 0;
private:
    int id_{};
    std::string title_;
    std::string date_;
    double price_{};
    std::string venueOrRoute_;
    std::vector<Seat> seats_;
};
