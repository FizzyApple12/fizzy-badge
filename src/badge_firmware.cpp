#include <math.h>
#include <time.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "pico/multicore.h"
#include <cstdarg>

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

#include "config.hpp"
#include "common.hpp"
#include "bag.hpp"

using namespace pimoroni;

static Hub75 hub75(HUB75_WIDTH, HUB75_HEIGHT, nullptr, PANEL_GENERIC, true);

static PicoGraphics_PenRGB888 graphics1(hub75.width, hub75.height, nullptr);
static PicoGraphics_PenRGB888 graphics2(hub75.width, hub75.height, nullptr);

void __isr dma_complete() {
    hub75.dma_complete();
}

#include "effects/effects.h"

typedef void (*init_function)();
typedef void (*effect_function)(PicoGraphics_PenRGB888);
typedef void (*reset_function)();

void no_reset() {}

#define NUM_EFFECTS 10

static int effect_bag[NUM_EFFECTS];
static int effect_bag_pointer = NUM_EFFECTS + 1;

static init_function effects_init[NUM_EFFECTS] = {
    MRRFLogo::init,
    QuadranglesLogo::init,
    FizzyLogo::init,
    DrDaveFunny::init,
    NyanCat::init,
//    RickRoll::init,
    Shader1::init,
    Shader2::init,
    Shader3::init,
    Shader4::init,
    Shader5::init
};

static effect_function effects[NUM_EFFECTS] = {
    MRRFLogo::main,
    QuadranglesLogo::main,
    FizzyLogo::main,
    DrDaveFunny::main,
    NyanCat::main,
//    RickRoll::main,
    Shader1::main,
    Shader2::main,
    Shader3::main,
    Shader4::main,
    Shader5::main
};

static reset_function effects_reset[NUM_EFFECTS] = {
    no_reset,
    no_reset,
    no_reset,
    no_reset,
    no_reset,
    no_reset,
    no_reset,
    no_reset,
    no_reset,
    Shader5::init
};

static uint8_t pre_frame[HUB75_WIDTH * HUB75_HEIGHT * sizeof(uint32_t)];

#define NUM_TRANSITIONS 10

static int transition_bag[NUM_TRANSITIONS];
static int transition_bag_pointer = NUM_TRANSITIONS + 1;

static int run_num = 0;

static clock_t transition_start;

#include "transitions.cpp"

typedef void (*transition_function)(PicoGraphics_PenRGB888, clock_t, uint8_t*, uint8_t*);

static transition_function transitions[NUM_TRANSITIONS] = {
    Transitions::swipe_left,
    Transitions::swipe_right,
    Transitions::swipe_up,
    Transitions::swipe_down,
    Transitions::reveal_left,
    Transitions::reveal_right,
    Transitions::reveal_up,
    Transitions::reveal_down,
    Transitions::key_in,
    Transitions::key_out,
};

bool run_transition(PicoGraphics_PenRGB888 from_graphics, PicoGraphics_PenRGB888 to_graphics) {
    if (run_num == 0) {
        transition_start = clock();
            
        bag_pick_next(transition_bag, &transition_bag_pointer, NUM_TRANSITIONS);
    }

    run_num++;
    
    clock_t time_spent = clock() - transition_start;

    memcpy(pre_frame, from_graphics.frame_buffer, HUB75_WIDTH * HUB75_HEIGHT * sizeof(uint32_t));

    transitions[bag_pick_current(transition_bag, transition_bag_pointer)](from_graphics, time_spent, pre_frame, (uint8_t*) to_graphics.frame_buffer);

    if (time_spent >= TRANSITION_MILLIS) {
        run_num = 0;
        return true;
    }

    return false;
}

#include "juggler.cpp"

int main() {
    set_sys_clock_khz(210000, true);

    stdio_init_all();

    hub75.start(dma_complete);
    graphics1.set_font("bitmap8");
    graphics2.set_font("bitmap8");

    graphics1.set_pen(0, 0, 0);
    graphics1.clear();

    graphics1.set_pen(0xff, 0xff, 0xff);
    graphics1.text("Fizzy Badge", {0, 0}, 64, 1);
    graphics1.text(std::string("HW v") + HW_REV, {0, 18}, 64, 1);
    graphics1.text(std::string("FW v") + FW_REV, {0, 27}, 64, 1);
    graphics1.text("EFFECTS", {0, 45}, 64, 1);
    graphics1.text("MULTICORE", {0, 54}, 64, 1);

    hub75.update(&graphics1);

    graphics1.set_pen(0x00, 0xff, 0x00);

    for (int i = 0; i < NUM_EFFECTS; i++) {
        effects_init[i]();
    }

    graphics1.text("EFFECTS", {0, 45}, 64, 1);
    hub75.update(&graphics1);

    multicore_launch_core1(juggler_thread_core1);

    graphics1.text("MULTICORE", {0, 54}, 64, 1);
    hub75.update(&graphics1);

    sleep_ms(3000);

    juggler_thread_core0();
}

extern "C" {

void __attribute__((noreturn)) __printflike(1, 0) usr_panic(const char *fmt, ...) {
    graphics1.set_pen(0, 0, 0);
    graphics1.clear();

    char message[512];

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, 512, fmt, args);
    va_end(args);

    graphics1.set_pen(0xff, 0x00, 0x00);
    graphics1.text(message, {0, 9}, 64, 1);
    
    hub75.update(&graphics1);

    int i;

    while (true) {
        i = (clock() / 500) % 2 * 0xff;

        graphics1.set_pen(i, 0x00, 0x00);
        graphics1.text("!!! PANIC !!!", {12, 0}, 64, 1);

        hub75.update(&graphics1);
    }
}

}