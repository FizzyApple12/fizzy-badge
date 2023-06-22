#include "pico/stdlib.h"
#include <time.h>
#include <string.h>
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

#define NUM_QUOTES 5

namespace Shader5 {
    std::string quotes[NUM_QUOTES] = {
        "RepRap, yeah!",
        "Go \nQuadrangles!",
        "Go ICE \nRobotics!",
        "Something clever",
        "Why did I \nspend 4 days making this?"
    };

    int selectedQuote = 0;
    int run_num = 0;

    int quote_bag[NUM_QUOTES];
    int quote_bag_pointer = NUM_QUOTES + 1;

    clock_t start_time;

    void init() {
        selectedQuote = bag_pick_next(quote_bag, &quote_bag_pointer, NUM_QUOTES);

        run_num = 0;
    }

    void main(PicoGraphics_PenRGB888 graphics) {
        if (run_num == 0) {
            graphics.set_pen(0, 0, 0);

            graphics.clear();

            start_time = clock();
        }

        run_num++;

        graphics.set_pen(0xff, 0xff, 0xff);
        
        graphics.text("Fizzy Says:", {0, 0}, 64, 1);

        if (clock() - start_time >= 2500) {
            graphics.text(quotes[selectedQuote], {0, 9}, 63, 1);
        }
    }

}