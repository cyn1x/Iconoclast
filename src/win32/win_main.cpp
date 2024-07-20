#ifndef UNICODE
#define UNICODE
#endif

#include "../app.h"
#include "../audio.h"
#include "../renderer.h"
#include "win_input.h"
#include "win_profiler.h"
#include "win_sound.h"
#include "win_window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

    HWND hwnd = Win32InitWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    HDC  hdc  = GetDC(hwnd);

    platform_graphics_buffer graphicsBuffer  = {};
    platform_sound_buffer    soundBuffer     = {};
    platform_input           controllerInput = {};

    Win32InitDSound(hwnd, &soundBuffer);
    PlatformInitAudio(&soundBuffer);

    Running                 = true;
    win32_profiler profiler = {};
    Win32StartProfiler(&profiler);
    while (Running) {
        MSG msg = {0};

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                Running = false;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        Win32UpdateInput(&controllerInput);
        Win32UpdateAudio(&soundBuffer);
        Win32UpdateGraphics(&graphicsBuffer);
        PlatformUpdate(&graphicsBuffer, &soundBuffer, &controllerInput);
        Win32UpdateSound(&soundBuffer);
        Win32UpdateWindow(hwnd, hdc);
        Win32UpdateProfiler(&profiler);
    }
}
