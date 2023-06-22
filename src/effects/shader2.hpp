#include "pico/stdlib.h"
#include <time.h>
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

#define SHADER2_PERIOD 512

namespace Shader2 {
    int i = 0;
    int speed = 1;

    int color = 0;

    uint8_t hue_map[SHADER2_PERIOD][3];
    uint8_t offset_map[HUB75_WIDTH];

    void init() {
        for (uint i = 0; i < SHADER2_PERIOD; i++) {
            RGB p = RGB::from_hsv(i / (float) SHADER2_PERIOD, 1.0f, 0.7f);
            hue_map[i][0] = p.r;
            hue_map[i][1] = p.g;
            hue_map[i][2] = p.b;
        }

        for (uint i = 0; i < HUB75_WIDTH; i++) {
            offset_map[i] = get_rand_32() % HUB75_HEIGHT;
        }
    }

    void main(PicoGraphics_PenRGB888 graphics) {
        i += speed;

        if (i > HUB75_WIDTH)
            i -= HUB75_WIDTH;

        color += speed;

        if (color >= SHADER2_PERIOD)
            color -= SHADER2_PERIOD;

        for (uint x = 0; x < HUB75_WIDTH; x++) {
            for (uint y = 0; y < HUB75_HEIGHT; y++) {
                float multiplier = (((y - i - offset_map[x]) % HUB75_HEIGHT) / (float) HUB75_HEIGHT);

                if (multiplier >= 0.98f) graphics.set_pen(0xff, 0xff, 0xff);
                else graphics.set_pen(hue_map[color][0] * multiplier, hue_map[color][1] * multiplier, hue_map[color][2] * multiplier);

                graphics.pixel(Point(x, y));
            }
        }

        //sleep_ms(15);
    }

}