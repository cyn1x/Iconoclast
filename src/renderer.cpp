#include "renderer.h"
#include "types.h"

void Render(platform_graphics *graphics)
{
    uint8 *row = (uint8 *)graphics->buffer;
    for (int y = 0; y < graphics->height; ++y) {

        uint8 *pixel = (uint8 *)row;
        for (int x = 0; x < graphics->width; ++x) {
            *pixel = 255;
            ++pixel;

            *pixel = 255;
            ++pixel;

            *pixel = 255;
            ++pixel;

            *pixel = 0;
            ++pixel;
        }

        row += graphics->pitch;
    }
}
