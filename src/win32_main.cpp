#ifndef UNICODE
#define UNICODE
#endif

#include "input.h"
#include "profiler.h"
#include "renderer.h"
#include "sound.h"
#include "window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

    HWND hwnd = Win32InitWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    HDC  hdc  = GetDC(hwnd);

    Win32InitDSound(hwnd);

    Running       = true;
    profiler prof = {};
    Win32StartProfiler(&prof);
    while (Running) {
        MSG msg = {0};

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                Running = false;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        offscreen_buffer backbuffer = {};
        backbuffer.memory           = Win32Backbuffer.memory;
        backbuffer.width            = Win32Backbuffer.width;
        backbuffer.height           = Win32Backbuffer.height;
        backbuffer.pitch            = Win32Backbuffer.pitch;

        HandleXInput();
        Render(&backbuffer);
        Win32PlaySound();
        Win32UpdateWindow(hwnd, hdc);
        Win32UpdateProfiler(&prof);
    }
}
