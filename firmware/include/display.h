#pragma once

#include <cmath>
#include <cstdint>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

extern Adafruit_SSD1331 g_display_device;

struct DisplayArea {
    uint16_t px, py;
    uint16_t w, h;
};

struct Display {
    uint16_t w;
    uint16_t h;
    DisplayArea status_line;
    DisplayArea content;
};

extern const Display g_display;

constexpr uint16_t rgb2bytes(float r, float g, float b) {
    uint16_t r_bytes = static_cast<uint16_t>(r * pow(2.f, 5.f));
    uint16_t g_bytes = static_cast<uint16_t>(g * pow(2.f, 6.f));
    uint16_t b_bytes = static_cast<uint16_t>(b * pow(2.f, 5.f));
    
    uint16_t result = r_bytes;
    result <<= 6;
    result |= g_bytes;
    result <<= 5;
    result |= b_bytes;
    return result;
}

constexpr uint16_t rgb2bytes_test(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t result = 0;
    result |= r >> (8 - 5);
    result <<= 6;
    result |= g >> (8 - 6);
    result <<= 5;
    result |= b >> (8 - 5);
    return result;
}

constexpr uint16_t rgb2bytes(uint8_t rgb[3]) {
    return rgb2bytes(
        static_cast<float>(rgb[0]) / 255.f,
        static_cast<float>(rgb[1]) / 255.f,
        static_cast<float>(rgb[2]) / 255.f
    );
}

constexpr uint16_t BLACK = 0;
constexpr uint16_t RED   = rgb2bytes(1, 0, 0);
constexpr uint16_t GREEN = rgb2bytes(0, 1, 0);
constexpr uint16_t BLUE  = rgb2bytes(0, 0, 1);
constexpr uint16_t WHITE = rgb2bytes(1, 1, 1);
