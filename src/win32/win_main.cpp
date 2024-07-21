#ifndef UNICODE
#define UNICODE
#endif

#include "../app.h"
#include "win_input.h"
#include "win_profiler.h"
#include "win_sound.h"
#include "win_system.h"
#include "win_window.h"

void       AllocateStorage(platform_memory *memory, platform_audio *sound);
void       DeallocateStorage(platform_memory *memory, platform_audio *sound);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

    HWND hwnd = Win32InitWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    HDC  hdc  = GetDC(hwnd);

    platform_memory   memory   = {};
    platform_graphics graphics = {};
    platform_audio    sound    = {};
    platform_input    input    = {};

    Win32InitDSound(hwnd, &sound);
    PlatformInitMemory(&memory);
    PlatformInitAudio(&sound);

    AllocateStorage(&memory, &sound);

    Running                 = true;
    win32_profiler profiler = {};
    Win32StartProfiler(&profiler);
    while (Running) {
        MSG msg = {0};

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                Running = false;
                DeallocateStorage(&memory, &sound);

                ExitProcess(0);
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        Win32UpdateInput(&input);
        Win32UpdateAudio(&sound);
        Win32UpdateGraphics(&graphics);
        PlatformUpdate(&graphics, &sound, &input);
        Win32UpdateSound(&sound);
        Win32UpdateWindow(hwnd, hdc);
        Win32UpdateProfiler(&profiler);
    }
}
