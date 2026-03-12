#include "BookingSystem.h"
#include "Exceptions.h"

#include <clocale>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace {

constexpr int MAX_MENU_ITEM = BookingSystem::MAX_MENU_ITEM;
constexpr const char* SEPARATOR = "------------------------------------------------------------";

void setupConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::setlocale(LC_ALL, "");
}

void printHeader(const std::string& title) {
    std::cout << '\n' << SEPARATOR << '\n';
    std::cout << title << '\n';
    std::cout << SEPARATOR << '\n';
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value{};
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter an integer.\n";
    }
}
