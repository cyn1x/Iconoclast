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

void       Win32ProcessPendingMessages();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

    HWND hwnd = Win32InitWindow(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    HDC  hdc  = GetDC(hwnd);

    game_memory   memory   = {};
    game_graphics graphics = {};
    game_audio    audio    = {};
    game_input    input    = {};

    Win32InitDSound(hwnd, &audio);
    GameInitAudio(&audio);
    Win32AllocateMemory(&memory, &audio);
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

        Win32ProcessPendingMessages();
        if (!Running) {
            // TODO: Handle cleanup process before exiting
            Win32DeallocateMemory(&memory, &audio);

            ExitProcess(0);
        }

        Win32UpdateInput(&input);
        Win32UpdateAudio(&audio);
        Win32UpdateGraphics(&graphics);
        GameUpdate(&graphics, &audio, &input, &memory);
        Win32UpdateSound(&audio);
        Win32UpdateWindow(hwnd, hdc);
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
                Win32HandleKeyInput(msg);
                break;
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}
