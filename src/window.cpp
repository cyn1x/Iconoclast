#include "window.h"

static win32_window_dimensions GetWindowDimensions(HWND hwnd)
{
    win32_window_dimensions result;
    
    RECT rc;
    GetClientRect(hwnd, &rc);
    result.width = rc.right - rc.left;
    result.height = rc.bottom - rc.top;
    
    return result;
}

static void Win32CopyBufferToWindow(win32_offscreen_buffer *buffer, HDC hdc, int windowWidth, int windowHeight)
{
    StretchDIBits(hdc,
                  0, 0, windowWidth, windowHeight,
                  0, 0, buffer->width, buffer->height,
                  buffer->memory,
                  &buffer->info,
                  DIB_RGB_COLORS, SRCCOPY);
}


static void Win32ResizeDIBSection(win32_offscreen_buffer *buffer, int width, int height)
{
    if(buffer->memory) {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }
    
    int bytesPerPixel = 4;
    
    buffer->width = width;
    buffer->height = height;
    
    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = -buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;
    
    int bitmapMemorySize = (buffer->width * buffer->height) * bytesPerPixel;
    buffer->memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    buffer->pitch = width * bytesPerPixel;
}

static HWND Win32CreateWindow(HINSTANCE hInstance)
{
    const char CLASS_NAME[]  = "Rakija Engine Window Class";
    
    WNDCLASSA wc = {0};
    
    Win32ResizeDIBSection(&Backbuffer, 1280, 720);
    
    wc.style = CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClassA(&wc);
    
    HWND hwnd = CreateWindowExA(
                               0,
                               CLASS_NAME,
                               "Rakija Engine",
                               WS_OVERLAPPEDWINDOW,
                               
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               
                               NULL,
                               NULL,
                               hInstance,
                               NULL
                               );

    return hwnd;
}

int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

    HWND hwnd = Win32CreateWindow(hInstance); 
    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    HDC hdc = GetDC(hwnd);

    Running = true;
    while(Running) {
        MSG msg = {0};
        
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {    
            if(msg.message == WM_QUIT) {
                Running = false;
            }
            
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        HandleXInput(); 

        Render(&Backbuffer);
        
        win32_window_dimensions dimensions = GetWindowDimensions(hwnd);
        Win32CopyBufferToWindow(&Backbuffer, hdc, dimensions.width, dimensions.height);
    }
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_SIZE:
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                int x = ps.rcPaint.left;
                int y = ps.rcPaint.top;
                int w = ps.rcPaint.right - ps.rcPaint.left;
                int h = ps.rcPaint.bottom - ps.rcPaint.top;

                win32_window_dimensions dimensions = GetWindowDimensions(hwnd);
                Win32CopyBufferToWindow(&Backbuffer, hdc, dimensions.width, dimensions.height);

                EndPaint(hwnd, &ps);
            }
            return 0;

        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        case WM_KEYUP:
            HandleKeyInput(wParam, lParam);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
