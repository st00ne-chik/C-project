include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(ticket_booking_tests BookingSystemTests.cpp)
target_link_libraries(ticket_booking_tests PRIVATE ticket_booking_core GTest::gtest_main)
target_include_directories(ticket_booking_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(ticket_booking_tests)
