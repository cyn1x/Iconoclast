#ifndef UNICODE
#define UNICODE
#endif

#include "../game.h"
#include "../platform.h"
#include "win_input.h"
#include "win_profiler.h"
#include "win_sound.h"
#include "win_system.h"
#include "win_window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

    HWND hwnd = Win32InitWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    HDC  hdc  = GetDC(hwnd);

    game_memory   memory   = {};
    game_graphics graphics = {};
    game_audio    sound    = {};
    game_input    input    = {};

    Win32InitDSound(hwnd, &sound);
    GameInitAudio(&sound);
    Win32AllocateMemory(&memory, &sound);
    blob blob = PlatformReadBlob((char *)__FILE__);
    if (blob.size > 0) {
        PlatformWriteBlob((char *)"test.out", &blob);
    }
    PlatformFreeBlob(&blob);
    blob                    = {};

    Running                 = true;
    win32_profiler profiler = {};
    Win32StartProfiler(&profiler);
    while (Running) {
        MSG msg = {0};

        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                Running = false;
                Win32DeallocateMemory(&memory, &sound);

                ExitProcess(0);
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        Win32UpdateInput(&input);
        Win32UpdateAudio(&sound);
        Win32UpdateGraphics(&graphics);
        GameUpdate(&graphics, &sound, &input, &memory);
        Win32UpdateSound(&sound);
        Win32UpdateWindow(hwnd, hdc);
        Win32UpdateProfiler(&profiler);
    }
}
