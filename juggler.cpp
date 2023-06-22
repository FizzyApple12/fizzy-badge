#include "pico/stdlib.h"
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

struct juggler_state {
    int rx_val;
    int state; // 0 waiting, 1 readying, 2 rendering, 3 rendering and transitioning
    int currentEffect;
    bool hub75_control;
    clock_t effect_start_time;
};

static juggler_state core0_state = {
    0,
    2,
    0,
    true,
    0
};

static juggler_state core1_state = {
    0,
    0,
    0,
    false,
    0
};

void core0_sio_irq() {
    while (multicore_fifo_rvalid()) {
        core0_state.rx_val = multicore_fifo_pop_blocking();

        if (core0_state.rx_val >= 10) {
            core0_state.hub75_control = true;
        } else {
            core0_state.state = core0_state.rx_val;
        }

        core0_state.effect_start_time = clock();
    }

    multicore_fifo_clear_irq();
}

void core1_sio_irq() {
    while (multicore_fifo_rvalid()) {
        core1_state.rx_val = multicore_fifo_pop_blocking();

        if (core1_state.rx_val >= 10) {
            core1_state.hub75_control = true;
        } else {
            core1_state.state = core1_state.rx_val;
        }

        core1_state.effect_start_time = clock();
    }

    multicore_fifo_clear_irq();
}

void juggler_thread_core0() {
    set_sys_clock_khz(210000, true);

    multicore_fifo_clear_irq();

    irq_set_exclusive_handler(SIO_IRQ_PROC0, core0_sio_irq);
    irq_set_enabled(SIO_IRQ_PROC0, true);

    core0_state.currentEffect = bag_pick_next(effect_bag, &effect_bag_pointer, NUM_EFFECTS);
    core0_state.effect_start_time = clock();

    while (true) {
        if (core0_state.state == 1) {
            core0_state.currentEffect = bag_pick_next(effect_bag, &effect_bag_pointer, NUM_EFFECTS);

            core0_state.state = 0;
        }

        if (core0_state.state > 1) {
            effects[core0_state.currentEffect](graphics1);

            if (core0_state.state > 2) {
                if (run_transition(graphics1, graphics2)) {
                    core0_state.hub75_control = false;
                    effects_reset[core0_state.currentEffect]();
                    multicore_fifo_push_blocking(15);
                    core0_state.state = 1;
                }
            }

            if (core0_state.state == 2 && clock() - core0_state.effect_start_time >= EFFECT_MILLIS) {
                core0_state.effect_start_time = clock();
                multicore_fifo_push_blocking(2);
                core0_state.state = 3;
            }
        }

        if (core0_state.hub75_control)
            hub75.update(&graphics1);
    }
}

void juggler_thread_core1() {
    set_sys_clock_khz(210000, true);

    multicore_fifo_clear_irq();

    irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_sio_irq);
    irq_set_enabled(SIO_IRQ_PROC1, true);

    core1_state.currentEffect = bag_pick_next(effect_bag, &effect_bag_pointer, NUM_EFFECTS);
    core1_state.effect_start_time = clock();

    while (true) {
        if (core1_state.state == 1) {
            core1_state.currentEffect = bag_pick_next(effect_bag, &effect_bag_pointer, NUM_EFFECTS);

            core1_state.state = 0;
        }

        if (core1_state.state > 1) {
            effects[core1_state.currentEffect](graphics2);

            if (core1_state.state > 2) {
                if (run_transition(graphics2, graphics1)) {
                    core1_state.hub75_control = false;
                    effects_reset[core1_state.currentEffect]();
                    multicore_fifo_push_blocking(15);
                    core1_state.state = 1;
                }
            }

            if (core1_state.state == 2 && clock() - core1_state.effect_start_time >= EFFECT_MILLIS) {
                core1_state.effect_start_time = clock();
                multicore_fifo_push_blocking(2);
                core1_state.state = 3;
            }
        }

        if (core1_state.hub75_control)
            hub75.update(&graphics2);
    }
}