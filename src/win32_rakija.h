#ifndef WIN32_RAKIJA_H
#define WIN32_RAKIJA_H

#include "core.h"

struct win32_offscreen_buffer
{
    BITMAPINFO info;
    void *memory;
    int width;
    int height;
    int pitch;
};

struct win32_window_dimensions
{
    int width;
    int height;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //WIN32_RAKIJA_H
