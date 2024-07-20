#ifndef WIN_WINDOW_H
#define WIN_WINDOW_H

#include "../renderer.h"
#include <windows.h>

struct win32_offscreen_buffer
{
    BITMAPINFO info;
    void      *memory;
    int        width;
    int        height;
    int        pitch;
};

struct win32_window_dimensions
{
    int width;
    int height;
};

extern struct win32_offscreen_buffer Win32Backbuffer;
struct win32_window_dimensions       Win32GetWindowDimensions(HWND hwnd);

// FIX: Restrict applicable callers
static bool Running;

HWND        Win32InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                            PWSTR pCmdLine, int nCmdShow);
void        Win32UpdateWindow(HWND hwnd, HDC hdc);
void Win32UpdateGraphics(struct platform_graphics_buffer *platformBuffer);

#endif // !WIN_WINDOW_H
