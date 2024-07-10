#include "renderer.h"
#include <stdint.h>

void Render(win32_offscreen_buffer *buffer)
{
    uint8_t *row = (uint8_t *)buffer->memory;
    for (int y = 0; y < buffer->height; ++y) {

        uint8_t *pixel = (uint8_t *)row;
        for (int x = 0; x < buffer->width; ++x) {
            *pixel = 255;
            ++pixel;

            *pixel = 255;
            ++pixel;

            *pixel = 255;
            ++pixel;

            *pixel = 0;
            ++pixel;
        }

        row += buffer->pitch;
    }
}
