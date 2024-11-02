#pragma once

#include <array>
#include <cinttypes>
#include <cstdint>
#include <cstdlib>

#include <ESP8266TrueRandom.h>

extern ESP8266TrueRandomClass g_rng;

struct DiceDescription {
    uint32_t size;
    uint32_t min_value;
    uint32_t max_value;
    std::array<char, 32> name;
    uint8_t button;

    DiceDescription(uint32_t size, uint8_t button)
        : size{size}
        , min_value{1}
        , max_value{size}
        , button{button}
    {
        snprintf(name.data(), name.size(), "D%" PRIu32, size);
        if (max_value < 10) {
            name[2] = ' ';
            name[3] = '\0';
        }
    }
    
    uint32_t roll() const {
        return g_rng.random(min_value, max_value + 1);
    }
};

constexpr size_t NUM_DICE = 6;
constexpr size_t D4i = 0;
constexpr size_t D6i = 1;
constexpr size_t D8i = 2;
constexpr size_t D10i = 3;
constexpr size_t D12i = 4;
constexpr size_t D20i = 5;

std::array<DiceDescription, NUM_DICE> const DICE_DESCRIPTION{
    DiceDescription{ 4,  0b001 },
    DiceDescription{ 6,  0b010 },
    DiceDescription{ 8,  0b011 },
    DiceDescription{ 10, 0b100 },
    DiceDescription{ 12, 0b101 },
    DiceDescription{ 20, 0b110 },
};

DiceDescription const DICE_D100{ 100, 0 };


inline uint32_t roll_d100() {
    auto const& d10 = DICE_DESCRIPTION[D10i];

    uint32_t tens = d10.roll() - 1;
    uint32_t units = d10.roll();

    return tens * 10 + units;
}
