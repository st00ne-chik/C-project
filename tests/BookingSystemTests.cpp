#include "BookingSystem.h"
#include "Exceptions.h"

#include <gtest/gtest.h>
#include <variant>

class BookingSystemTest : public ::testing::Test {
protected:
    BookingSystem system;

    void SetUp() override {
        system.seedDemoData();
    }
};

TEST_F(BookingSystemTest, DemoDataContainsSixEvents) {
    EXPECT_EQ(system.getEvents().size(), 6);
}

TEST_F(BookingSystemTest, FindEventByIdWorksForExistingAndMissingId) {
    EXPECT_TRUE(system.findEventById(1).has_value());
    EXPECT_FALSE(system.findEventById(999).has_value());
}

TEST_F(BookingSystemTest, FilterByTypeReturnsExpectedCounts) {
    EXPECT_EQ(system.getEventsByType(EventTypeFilter::Concert).size(), 2);
    EXPECT_EQ(system.getEventsByType(EventTypeFilter::MovieSession).size(), 2);
    EXPECT_EQ(system.getEventsByType(EventTypeFilter::TrainTrip).size(), 2);
}

TEST_F(BookingSystemTest, BookSeatCreatesBookingAndChangesFreeSeatsCount) {
    const int before = system.countFreeSeats(1);

    const Booking booking = system.bookSeat(1, 1, "Alice");
    const int after = system.countFreeSeats(1);

    EXPECT_EQ(booking.bookingId, 1);
    EXPECT_EQ(booking.status, BookingStatus::Active);
    EXPECT_EQ(after, before - 1);
}

TEST_F(BookingSystemTest, BookingSameSeatTwiceThrowsException) {
    system.bookSeat(1, 2, "Alice");
    EXPECT_THROW(system.bookSeat(1, 2, "Bob"), SeatAlreadyBookedException);
}

TEST_F(BookingSystemTest, CancelBookingFreesSeatAndChangesStatus) {
    const Booking booking = system.bookSeat(1, 3, "Charlie");
    system.cancelBooking(booking.bookingId);

    const auto updatedBooking = system.findBookingById(booking.bookingId);
    ASSERT_TRUE(updatedBooking.has_value());
    EXPECT_EQ(updatedBooking->status, BookingStatus::Canceled);

    const auto firstFreeSeat = system.findFirstFreeSeat(1);
    EXPECT_TRUE(firstFreeSeat.has_value());
    EXPECT_EQ(*firstFreeSeat, 1);
}
