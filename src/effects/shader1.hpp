#include "pico/stdlib.h"
#include <time.h>

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

namespace Shader1 {
    uint8_t hue_map[HUB75_WIDTH][3];

    int i = 0;
    int speed = 1;

    void init() {
        for (uint i = 0; i < HUB75_WIDTH; i++) {
            RGB p = RGB::from_hsv(i / (float) HUB75_WIDTH, 1.0f, 0.7f);
            hue_map[i][0] = p.r;
            hue_map[i][1] = p.g;
            hue_map[i][2] = p.b;
        }
    }

    void main(PicoGraphics_PenRGB888 graphics) {
        i = clock() / 15;

        // if (i > HUB75_WIDTH)
        //     i -= HUB75_WIDTH;

        for (uint x = 0; x < HUB75_WIDTH; x++) {
            for (uint y = 0; y < HUB75_HEIGHT; y++) {
                uint8_t r = hue_map[(x + i) % HUB75_WIDTH][0];
                uint8_t g = hue_map[(x + i) % HUB75_WIDTH][1];
                uint8_t b = hue_map[(x + i) % HUB75_WIDTH][2];

                graphics.set_pen(r, g, b);
                graphics.pixel(Point(x, y));
            }
        }
    }

}