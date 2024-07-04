#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

#include "input.h"
#include "renderer.h"
#include "sound.h"

struct win32_window_dimensions
{
    int width;
    int height;
};

// FIX: Restrict applicable callers
static bool      Running;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
        int nCmdShow);

#endif // WINDOW_H
