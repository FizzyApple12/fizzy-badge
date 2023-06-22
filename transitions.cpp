#include "pico/stdlib.h"
#include <time.h>
#include <string.h>
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

namespace Transitions {
    void swipe_left(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 64);

        draw_screenshot(graphics, from, -x, 0);
        draw_screenshot(graphics, to, 64 - x, 0);
    }

    void swipe_right(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = 64 - map(time, 0, 1000, 0, 64);

        draw_screenshot(graphics, to, -x, 0);
        draw_screenshot(graphics, from, 64 - x, 0);
    }

    void swipe_up(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 64);

        draw_screenshot(graphics, from, -x, 0);
        draw_screenshot(graphics, to, 64 - x, 0);
    }

    void swipe_down(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = 64 - map(time, 0, 1000, 0, 64);

        draw_screenshot(graphics, to, 0, -x);
        draw_screenshot(graphics, from, 0, 64 - x);
    }

    void reveal_left(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 64);

        graphics.set_clip({0, 0, x, 64});

        draw_screenshot(graphics, to, 0, 0);

        graphics.remove_clip();
    }

    void reveal_right(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 64);

        graphics.set_clip({64 - x, 0, x, 64});

        draw_screenshot(graphics, to, 0, 0);

        graphics.remove_clip();
    }

    void reveal_up(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 64);

        graphics.set_clip({0, 0, 64, x});

        draw_screenshot(graphics, to, 0, 0);

        graphics.remove_clip();
    }

    void reveal_down(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 64);

        graphics.set_clip({0, 64 - x, 64, x});

        draw_screenshot(graphics, to, 0, 0);

        graphics.remove_clip();
    }

    void key_in(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = map(time, 0, 1000, 0, 32);

        graphics.set_clip({32 - x, 32 - x, x * 2, x * 2});

        draw_screenshot(graphics, to, 0, 0);

        graphics.remove_clip();
    }

    void key_out(PicoGraphics_PenRGB888 graphics, clock_t time, uint8_t* from, uint8_t* to) {
        int x = 32 - map(time, 0, 1000, 0, 32);

        draw_screenshot(graphics, to, 0, 0);

        graphics.set_clip({32 - x, 32 - x, x * 2, x * 2});

        draw_screenshot(graphics, from, 0, 0);

        graphics.remove_clip();
    }

}