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
    output_sound_buffer soundbuffer = {};

    Win32InitDSound(hwnd);
    Win32SetPlatformData(&soundbuffer);
    PlatformInitAudio(&soundbuffer);

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

        output_graphics_buffer backbuffer = {};
        backbuffer.memory                 = Win32Backbuffer.memory;
        backbuffer.width                  = Win32Backbuffer.width;
        backbuffer.height                 = Win32Backbuffer.height;
        backbuffer.pitch                  = Win32Backbuffer.pitch;

        HandleXInput();
        Win32UpdateSound(&soundbuffer);
        GameUpdate(&backbuffer, &soundbuffer);
        Win32WriteSoundBuffer(&soundbuffer);
        Win32UpdateWindow(hwnd, hdc);
        Win32UpdateProfiler(&prof);
    }
}
