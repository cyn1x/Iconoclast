#ifndef WINDOW_H
#define WINDOW_H

#include "core.h"
#include "input.h"
#include "renderer.h"

struct win32_window_dimensions
{
    int width;
    int height;
};

static bool Running;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

#endif // WINDOW_H
