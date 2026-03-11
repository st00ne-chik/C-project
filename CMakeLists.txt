cmake_minimum_required(VERSION 3.20)
project(TicketBookingConsoleSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_library(ticket_booking_core
    src/Event.cpp
    src/BookingSystem.cpp
)

target_include_directories(ticket_booking_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(ticket_booking_core PUBLIC cxx_std_23)

add_executable(ticket_booking_app src/main.cpp)
target_link_libraries(ticket_booking_app PRIVATE ticket_booking_core)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
