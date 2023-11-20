#ifndef UNICODE
#define UNICODE
#endif

#include "win32_main.h"
#include "window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Run(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}
