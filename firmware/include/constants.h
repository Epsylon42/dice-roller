#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>

constexpr uint8_t PIN_CS   = 15;
constexpr uint8_t PIN_DC   = 16;

#if 1 == HARDWARE_VERSION
    constexpr uint8_t PIN_B1      = 5;
    constexpr uint8_t PIN_B2      = 4;
    constexpr uint8_t PIN_B3      = 2;
    constexpr uint8_t PIN_RST = 12;
#elif 2 == HARDWARE_VERSION
    constexpr uint8_t PIN_B1      = 12;
    constexpr uint8_t PIN_B2      = 5;
    constexpr uint8_t PIN_B3      = 2;
    constexpr uint8_t PIN_PERIPHERAL_EN = 5;
#else
#    ifdef HARDWARE_VERSION
#        error Unsupported HARDWARE_VERSION value
#    else
#        error HARDWARE_VERSION must be defined
#    endif
#endif

constexpr uint8_t PIN_MODE_SW = 0;
constexpr uint8_t PIN_CS1     = 1;
constexpr uint8_t PIN_CS2     = 3;
