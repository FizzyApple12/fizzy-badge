cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk/)
set(PIMORONI_PICO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pimoroni-pico/)

include(pico_sdk_import.cmake)
include(pimoroni-pico/pimoroni_pico_import.cmake)

project(badge_firmware C CXX ASM)
set(OUTPUT_NAME badge_firmware)

pico_sdk_init()

add_subdirectory(pimoroni-pico/common)
add_subdirectory(pimoroni-pico/drivers)
add_subdirectory(pimoroni-pico/libraries)

add_executable(${OUTPUT_NAME} 
    src/badge_firmware.cpp
    src/heatshrink/heatshrink_decoder.cpp
)

# enable usb output
pico_enable_stdio_usb(${OUTPUT_NAME} 1)

pico_add_extra_outputs(${OUTPUT_NAME})

target_compile_definitions(${OUTPUT_NAME} PRIVATE PICO_PANIC_FUNCTION=usr_panic)

target_include_directories(badge_firmware PRIVATE
    src/heatshrink
)

target_link_libraries(${OUTPUT_NAME}
    pico_stdlib
    pico_multicore
    pico_rand
    pico_divider
    pico_float
    hardware_pio
    hardware_dma
    pico_graphics
    hub75
    interstate75
)