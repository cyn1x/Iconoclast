#ifndef UNICODE
#define UNICODE
#endif

#include "Application.h"
#include <iostream>
#include <windows.h>

typedef Iconoclast::Application *(*CreateApplicationFunc)();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD            ThreadProc(LPVOID lpParam)
{
    // Get EXE handle
    HMODULE hExe = GetModuleHandle(NULL);
    if (!hExe) {
        std::cerr << "Failed to get EXE module handle!" << std::endl;
        return 1;
    }

    // Get the CreateApplication function pointer from the EXE
    auto CreateApplication =
        (CreateApplicationFunc)GetProcAddress(hExe, "CreateApplication");
    if (!CreateApplication) {
        std::cerr << "Failed to get CreateApplication from EXE!" << std::endl;
        return 1;
    }

    Iconoclast::Application *app = CreateApplication();
    if (app) {
        OutputDebugStringA("Running\n");
        app->Run();
    }

    delete app;

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    OutputDebugStringA("Running\n");
    // Perform actions based on the reason for calling.
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            CreateThread(NULL, 0, &ThreadProc, hinstDLL, NULL, NULL);
            break;

        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:

            if (lpvReserved != nullptr) {
                break; // do not do cleanup if process termination scenario
            }

            // Perform any necessary cleanup.
            break;
    }
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}
