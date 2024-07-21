#include "win_window.h"
#include "win_input.h"

struct win32_offscreen_buffer Win32Backbuffer = {};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static win32_window_dimensions Win32GetWindowDimensions(HWND hwnd)
{
    win32_window_dimensions result;

    RECT                    rc;
    GetClientRect(hwnd, &rc);
    result.width  = rc.right - rc.left;
    result.height = rc.bottom - rc.top;

    return result;
}

static void Win32CopyBufferToWindow(win32_offscreen_buffer *buffer, HDC hdc,
                                    int windowWidth, int windowHeight)
{
    StretchDIBits(hdc, 0, 0, windowWidth, windowHeight, 0, 0, buffer->width,
                  buffer->height, buffer->memory, &buffer->info, DIB_RGB_COLORS,
                  SRCCOPY);
}

static void Win32ResizeDIBSection(win32_offscreen_buffer *buffer, int width,
                                  int height)
{
    if (buffer->memory) {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }

    int bytesPerPixel                    = 4;

    buffer->width                        = width;
    buffer->height                       = height;

    buffer->info.bmiHeader.biSize        = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth       = buffer->width;
    buffer->info.bmiHeader.biHeight      = -buffer->height;
    buffer->info.bmiHeader.biPlanes      = 1;
    buffer->info.bmiHeader.biBitCount    = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;

    int bitmapMemorySize = (buffer->width * buffer->height) * bytesPerPixel;
    buffer->memory = VirtualAlloc(0, bitmapMemorySize, MEM_RESERVE | MEM_COMMIT,
                                  PAGE_READWRITE);
    buffer->pitch  = width * bytesPerPixel;
}

static HWND Win32CreateWindow(HINSTANCE hInstance)
{
    const char CLASS_NAME[] = "Iconoclast Engine Window Class";

    WNDCLASSA  wc           = {0};

    Win32ResizeDIBSection(&Win32Backbuffer, 1280, 720);

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Iconoclast Engine", WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL, NULL, hInstance, NULL);

    return hwnd;
}

HWND Win32InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     PWSTR pCmdLine, int nCmdShow)
{
    HWND hwnd = Win32CreateWindow(hInstance);
    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    return hwnd;
}

void Win32UpdateWindow(HWND hwnd, HDC hdc)
{
    win32_window_dimensions dimensions = Win32GetWindowDimensions(hwnd);
    Win32CopyBufferToWindow(&Win32Backbuffer, hdc, dimensions.width,
                            dimensions.height);
}

void Win32UpdateGraphics(struct platform_graphics *platformGraphics)
{
    platformGraphics->buffer = Win32Backbuffer.memory;
    platformGraphics->width  = Win32Backbuffer.width;
    platformGraphics->height = Win32Backbuffer.height;
    platformGraphics->pitch  = Win32Backbuffer.pitch;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_SIZE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT             ps;
        HDC                     hdc        = BeginPaint(hwnd, &ps);

        int                     x          = ps.rcPaint.left;
        int                     y          = ps.rcPaint.top;
        int                     w          = ps.rcPaint.right - ps.rcPaint.left;
        int                     h          = ps.rcPaint.bottom - ps.rcPaint.top;

        win32_window_dimensions dimensions = Win32GetWindowDimensions(hwnd);
        Win32CopyBufferToWindow(&Win32Backbuffer, hdc, dimensions.width,
                                dimensions.height);

        EndPaint(hwnd, &ps);
    }
        return 0;

    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_KEYUP:
        Win32HandleKeyInput(wParam, lParam);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
