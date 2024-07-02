#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <windows.h>

struct win32_offscreen_buffer
{
    BITMAPINFO info;
    void      *memory;
    int        width;
    int        height;
    int        pitch;
};

static win32_offscreen_buffer Backbuffer;

void                          Render(win32_offscreen_buffer *buffer);

#endif // RENDERER_H
