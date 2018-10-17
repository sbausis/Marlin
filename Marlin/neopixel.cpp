/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * neopixel.cpp
 */

#include "MarlinConfig.h"

#if ENABLED(NEOPIXEL_LED)

#include "neopixel.h"

#if ENABLED(NEOPIXEL_STARTUP_TEST)
  #include "utility.h"
#endif

Adafruit_NeoPixel pixels(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800);

#define NEOPIXEL_CASE
#if ENABLED(NEOPIXEL_CASE)
  #define NEOPIXEL_CASE_PIN          6        // LED driving pin on motherboard 4 => D4 (EXP2-5 on Printrboard) / 30 => PC7 (EXP3-13 on Rumba)
  #define NEOPIXEL_CASE_TYPE         NEO_GRB  // NEO_GRBW / NEO_GRB - four/three channel driver type (defined in Adafruit_NeoPixel.h)
  #define NEOPIXEL_CASE_BRIGHTNESS   65       // Initial brightness (0-255)
  #define NEOPIXEL_CASE_STARTUP_TEST          // Cycle through colors at startup
  #define NEOPIXEL_CASE_PIXELS       5        // Number of LEDs in the strip

#endif

Adafruit_NeoPixel pixels_case(NEOPIXEL_CASE_PIXELS, NEOPIXEL_CASE_PIN, NEOPIXEL_CASE_TYPE + NEO_KHZ800);

int pixels_case_group_0[] =  {0, 1, 2, 3, 4, -1};
int pixels_case_group_1[] =  {0, 1, 2, -1};
int pixels_case_group_2[] =  {3, 4, -1};

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel bus, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return bus.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return bus.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return bus.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void rainbow(Adafruit_NeoPixel bus, int* group, uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
	while (*group >= 0 && *group < bus.numPixels()) {
    bus.setPixelColor(i, Wheel(bus, (i+j) & 255));
    group++;
	}
    bus.show();
    safe_delay(wait);
  }
}

void set_neopixel_group_color(Adafruit_NeoPixel bus, int* group, const uint32_t color) {
  while (*group >= 0 && *group < bus.numPixels()) {
    bus.setPixelColor(*group, color);
    group++;
  }
}

void set_neopixel_color(Adafruit_NeoPixel bus, const uint32_t color) {
  set_neopixel_group_color(bus, pixels_case_group_0, color);
  //for (uint16_t i = 0; i < bus.numPixels(); ++i)
  //  bus.setPixelColor(i, color);
  //bus.show();
}

void setup_neopixel_case() {
  SET_OUTPUT(NEOPIXEL_CASE_PIN);
  pixels_case.setBrightness(NEOPIXEL_CASE_BRIGHTNESS); // 0 - 255 range
  pixels_case.begin();
  pixels_case.show(); // initialize to all off

  #if ENABLED(NEOPIXEL_CASE_STARTUP_TEST)
	set_neopixel_color(pixels_case, pixels_case.Color(NEO_WHITE));       // white
	rainbow(pixels_case, pixels_case_group_0, 20);
	safe_delay(500);
	set_neopixel_color(pixels_case, pixels_case.Color(NEO_WHITE));       // white
	rainbow(pixels_case, pixels_case_group_1, 20);
	safe_delay(500);
	set_neopixel_color(pixels_case, pixels_case.Color(NEO_WHITE));       // white
	rainbow(pixels_case, pixels_case_group_2, 20);
	safe_delay(500);
  #endif
  set_neopixel_color(pixels_case, pixels_case.Color(NEO_WHITE));       // white
}

/*void set_neopixel_color(const uint32_t color) {
  for (uint16_t i = 0; i < pixels.numPixels(); ++i)
    pixels.setPixelColor(i, color);
  pixels.show();
}*/

void setup_neopixel() {
  SET_OUTPUT(NEOPIXEL_PIN);
  pixels.setBrightness(NEOPIXEL_BRIGHTNESS); // 0 - 255 range
  pixels.begin();
  pixels.show(); // initialize to all off

  #if ENABLED(NEOPIXEL_STARTUP_TEST)
    //safe_delay(1000);
    set_neopixel_color(pixels, pixels.Color(255, 0, 0, 0));  // red
    safe_delay(1000);
    set_neopixel_color(pixels, pixels.Color(0, 255, 0, 0));  // green
    safe_delay(1000);
    set_neopixel_color(pixels, pixels.Color(0, 0, 255, 0));  // blue
    safe_delay(1000);
  #endif
  set_neopixel_color(pixels, pixels.Color(NEO_WHITE));       // white
  setup_neopixel_case();
}

#endif // NEOPIXEL_LED
