#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <windows.h>

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
