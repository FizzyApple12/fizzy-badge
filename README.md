# Fizzy Badge

A custom LED badge using a HUB75 display and a Pimoroni Interstate 75

## Renderer

The multi-core renderer built for this project is designed to make compositing between effects as smooth as possible.

Here's what the general flow looks like:

    core0                           core1
    rendering                       pick next effect and wait
    [time to render] -------------> [interrupt]
    compositing + rendering         rendering
    [transfer control] -----------> [interrupt]
    pick next effect and wait       rendering
    [interrupt] <------------------ [time to render]
    rendering                       compositing + rendering
    [interrupt] <------------------ [transfer control]

## Building

This project uses the [Pico SDK](https://github.com/raspberrypi/pico-sdk "Link to Pico SDK") and [Pimoroni's Pico Libraries](https://github.com/pimoroni/pimoroni-pico "Link to Pimoroni's Pico Libraries"), so make sure to have the dependencies that those need.

Also, make sure to initialize git submodules with ``git submodule update --init --recursive``

Outside of that, this simple command flow works to build everything:

    cmake -B build .
    cd build
    make
    cd ..

## Credits

* [Pico SDK](https://github.com/raspberrypi/pico-sdk "Link to Pico SDK")
* [Pimoroni's Pico Libraries](https://github.com/pimoroni/pimoroni-pico "Link to Pimoroni's Pico Libraries")
* [AtomicObject's Heatshrink](https://github.com/atomicobject/heatshrink "Link to AtomicObject's Heatshrink")
