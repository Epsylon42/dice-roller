#include <cstdint>

#include "modules/battery.h"

// blue = outline
// green = charge levels
// red = low charge mask
static const struct {
  uint8_t 	 width;
  uint8_t 	 height;
  uint8_t 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  uint8_t	 pixel_data[21 * 10 * 3 + 1];
} battery_bitmap = {
  21, 10, 3,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377"
  "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
  "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000"
  "\000\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\377\000"
  "\000\377\000\000\000\377\342\000\377\342\000\377\000\000\377\253\000\377\253\000\377\000\000\377"
  "w\000\377w\000\377\000\000\377G\000\377G\000\377\000\000\377\034\000\377\034\000\000\000\000\000\000\377"
  "\000\000\000\000\000\000\000\000\377\000\000\377\000\000\000\377\342\000\377\342\000\377\000\000\377\253\000"
  "\377\253\000\377\000\000\377w\000\377w\000\377\000\000\377G\000\377G\000\377\000\000\377\034\000\377"
  "\034\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\000\377\342\000\377\342\000"
  "\377\000\000\377\253\000\377\253\000\377\000\000\377w\000\377w\000\377\000\000\377G\000\377G\000"
  "\377\000\000\377\034\000\377\034\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000"
  "\000\377\342\000\377\342\000\377\000\000\377\253\000\377\253\000\377\000\000\377w\000\377w\000"
  "\377\000\000\377G\000\377G\000\377\000\000\377\034\000\377\034\000\000\000\000\000\000\377\000\000\000\000\000"
  "\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\000\000\000\377"
  "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
  "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000",
};

namespace modules {

void Battery::update() {

}

void Battery::draw() {

}

}
