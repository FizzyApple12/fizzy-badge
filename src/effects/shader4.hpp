#include "pico/stdlib.h"
#include <time.h>
#include "pico/rand.h"

#include "libraries/pico_graphics/pico_graphics.hpp"
#include "libraries/interstate75/interstate75.hpp"

namespace Shader4 {
    void from_hsv(float h, float s, float v, uint8_t& r, uint8_t& g, uint8_t& b) {
        float i = floor(h * 6.0f);
        float f = h * 6.0f - i;
        v *= 255.0f;
        uint8_t p = v * (1.0f - s);
        uint8_t q = v * (1.0f - f * s);
        uint8_t t = v * (1.0f - (1.0f - f) * s);

        switch (int(i) % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        }
    }

    struct blob_t {
        float x, y;
        float r;
        float dx, dy;
    };

    constexpr int blob_count = 10;

    std::array<blob_t, blob_count> blobs;

    float hue = 0.0f;

    uint8_t dark_r, dark_g, dark_b;
    uint8_t mid_r, mid_g, mid_b;
    uint8_t bright_r, bright_g, bright_b;

    void init() {
        for (auto& blob : blobs) {
            blob.x = rand() % 32;
            blob.y = rand() % 32;
            blob.r = ((rand() % 40) / 10.0f) + 5.0f;
            blob.dx = ((rand() % 2) / 10.0f) - 0.05f;
            blob.dy = ((rand() % 3) / 10.0f) - 0.1f;
        }
    }

    void main(PicoGraphics_PenRGB888 graphics) {
        hue += 0.001f;

        from_hsv(hue, 1.0f, 0.3f, dark_r, dark_g, dark_b);
        from_hsv(hue, 1.0f, 0.6f, mid_r, mid_g, mid_b);
        from_hsv(hue, 1.0f, 1.0f, bright_r, bright_g, bright_b);

        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                float v = 0.0f;
                
                for (auto& blob : blobs) {
                    float r_sq = blob.r * blob.r;
                    float d_sq = (x - blob.x) * (x - blob.x) + (y - blob.y) * (y - blob.y);
                    if (d_sq <= r_sq) {
                        v += 1.0f - (d_sq / r_sq);
                    }
                }

                if (v >= 1.5f)
                    graphics.set_pen(bright_r, bright_g, bright_b);
                else if (v >= 1.25f)
                    graphics.set_pen(mid_r, mid_g, mid_b);
                else if (v >= 1.0f)
                    graphics.set_pen(dark_r, dark_g, dark_b);
                else
                    graphics.set_pen(0, 0, 0);


                graphics.pixel(Point(y * 2, x * 2));
                graphics.pixel(Point(y * 2 + 1, x * 2));
                graphics.pixel(Point(y * 2, x * 2 + 1));
                graphics.pixel(Point(y * 2 + 1, x * 2 + 1));
            }
        }


        for (auto& blob : blobs) {
            blob.x += blob.dx * 10.0f;
            blob.y += blob.dy * 10.0f;

            if (blob.x < 0.0f || blob.x >= 32) {
                blob.dx *= -1.0f;
            }
            if (blob.y < 0.0f || blob.y >= 32) {
                blob.dy *= -1.0f;
            }
        }
    }

}