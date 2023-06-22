#include "pico/rand.h"

#ifndef COMMON_HPP
#define COMMON_HPP

clock_t clock() {
    return (clock_t) time_us_64() / 1000;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void draw_screenshot(pimoroni::PicoGraphics_PenRGB888 target_graphics, uint8_t* screenshot, int tx, int ty) {
    for (uint x = 0; x < HUB75_WIDTH; x++) {
        for (uint y = 0; y < HUB75_HEIGHT; y++) {
            int byte_target = (x * sizeof(uint32_t)) + (y * sizeof(uint32_t) * HUB75_WIDTH);

            target_graphics.set_pen(screenshot[byte_target + 2], screenshot[byte_target + 1], screenshot[byte_target]);

            target_graphics.pixel(pimoroni::Point(x + tx, y + ty));
        }
    }
}

#endif