#ifndef UNICODE
#define UNICODE
#endif

#include "../game.h"
#include "../platform.h"
#include "../renderer.h"
#include "win_input.h"
#include "win_profiler.h"
#include "win_sound.h"
#include "win_system.h"
#include "win_window.h"

void       Win32ProcessPendingMessages();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    HWND hwnd = Win32InitWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    HDC  hdc  = GetDC(hwnd);

    game_memory   memory   = {};
    game_graphics graphics = {};
    game_audio    audio    = {};
    game_input    input    = {};

    Win32InitXInput();
    Win32InitDSound(hwnd, &audio);
    GameInitAudio(&audio);
    Win32AllocateMemory(&memory, &audio);

    // TODO: Abstract to a basic persistent data handler
    blob blob = PlatformReadBlob((char *)__FILE__);
    if (blob.size > 0) {
        PlatformWriteBlob((char *)"test.out", &blob);
    }
    PlatformFreeBlob(&blob);
    blob                    = {};

    Running                 = true;
    win32_profiler profiler = {};
    Win32StartProfiler(&profiler);

    int   hz = 60;
    float ms = 1000.0f / (float)hz;

    while (Running) {
        double start = Win32GetCurrentTime(&profiler);

        Win32ProcessPendingMessages();
        if (!Running) {
            // TODO: Handle cleanup process before exiting
            Win32DeallocateMemory(&memory, &audio);

            ExitProcess(0);
        }

        Win32UpdateXInput(&input);
        Win32UpdateDSound(&audio);
        Win32UpdateGraphics(&graphics);
        Win32UpdateWindow(hwnd, hdc);
        Update(&audio, &input, &memory);

        // Lock ms per frame to sync with monitor refresh rate
        double end     = Win32GetCurrentTime(&profiler);
        float  elapsed = (float)(start + ms - end);
        if (elapsed > 0) {
            // Set scheduler granularity to 1 ms
            if (timeBeginPeriod(1) == TIMERR_NOERROR) {
                Sleep((DWORD)(elapsed));
            } else {
                // TODO: Add logging
            }
        }

        Win32FillSoundBuffer(&audio);
        Render(&graphics);
        Win32UpdateProfiler(&profiler);
    }
}

void Win32ProcessPendingMessages()
{
    MSG msg = {0};
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {

        switch (msg.message) {
            case WM_QUIT:
                Running = false;

            case WM_SYSKEYUP:
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            case WM_KEYUP:
                Win32HandleKeyInput(&msg);
                break;
            default:
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
        }
    }
}
