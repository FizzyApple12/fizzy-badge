#include "pico/stdlib.h"
#include <time.h>
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

namespace Shader3 {
    float heat[4500] = { 0.0f };

    void set(int x, int y, float v) {
        heat[x + y * HUB75_WIDTH] = v;
    }

    float get(int x, int y) {
        x = x < 0 ? 0 : x;
        x = x >= HUB75_WIDTH ? HUB75_WIDTH - 1 : x;

        return heat[x + y * HUB75_WIDTH];
    }

    void init() {

    }

    void main(PicoGraphics_PenRGB888 graphics) {
        for (int y = 0; y < HUB75_HEIGHT; y++) {
            for (int x = 0; x < HUB75_WIDTH; x++) {
                float value = get(x, y);

                graphics.set_pen(0, 0, 0);
                if (value > 0.5f) {
                    graphics.set_pen(255, 255, 180);
                }
                else if (value > 0.4f) {
                    graphics.set_pen(220, 160, 0);
                }
                else if (value > 0.3f) {
                    graphics.set_pen(180, 30, 0);
                }
                else if (value > 0.22f) {
                    graphics.set_pen(20, 20, 20);
                }

                // if (landscape) {
                    graphics.pixel(Point(x, y));
                // }
                // else {
                //     graphics.pixel(Point(y, x));
                // }

                // update this pixel by averaging the below pixels
                float average = (get(x, y) + get(x, y + 2) + get(x, y + 1) + get(x - 1, y + 1) + get(x + 1, y + 1)) / 5.0f;

                // damping factor to ensure flame tapers out towards the top of the displays
                average *= 0.99f;

                // update the heat map with our newly averaged value
                set(x, y, average);
            }
        }

        // clear the bottom row and then add a new fire seed to it
        for (int x = 0; x < HUB75_WIDTH; x++) {
            set(x, HUB75_HEIGHT - 1, 0.0f);
        }

        // add a new random heat source
        for (int c = 0; c < 7; c++) {
            int px = (rand() % (HUB75_WIDTH - 4)) + 2;
            set(px, HUB75_HEIGHT - 2, 1.0f);
            set(px + 1, HUB75_HEIGHT - 2, 1.0f);
            set(px - 1, HUB75_HEIGHT - 2, 1.0f);
            set(px, HUB75_HEIGHT - 1, 1.0f);
            set(px + 1, HUB75_HEIGHT - 1, 1.0f);
            set(px - 1, HUB75_HEIGHT - 1, 1.0f);
        }
    }

}