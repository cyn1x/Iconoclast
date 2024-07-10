#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>

struct win32_offscreen_buffer
{
    BITMAPINFO info;
    void      *memory;
    int        width;
    int        height;
    int        pitch;
};

static struct win32_offscreen_buffer Backbuffer;

void Render(struct win32_offscreen_buffer *buffer);

#endif // RENDERER_H
