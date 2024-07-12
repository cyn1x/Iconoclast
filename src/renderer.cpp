#include "renderer.h"
#include "types.h"

void Render(offscreen_buffer *buffer)
{
    uint8 *row = (uint8 *)buffer->memory;
    for (int y = 0; y < buffer->height; ++y) {

        uint8 *pixel = (uint8 *)row;
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
