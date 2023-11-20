#ifndef UNICODE
#define UNICODE
#endif

#include "win32_rakija.h"
#include "input.h"

static bool Running;
static win32_offscreen_buffer Backbuffer;

win32_window_dimensions GetWindowDimensions(HWND hwnd)
{
    win32_window_dimensions result;
    
    RECT rc;
    GetClientRect(hwnd, &rc);
    result.width = rc.right - rc.left;
    result.height = rc.bottom - rc.top;
    
    return result;
}

static void Win32ResizeDIBSection(win32_offscreen_buffer *buffer, int width, int height)
{
    if(buffer->memory)
    {
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

static void Win32CopyBufferToWindow(win32_offscreen_buffer *buffer, HDC hdc, int windowWidth, int windowHeight)
{
    StretchDIBits(hdc,
                  0, 0, windowWidth, windowHeight,
                  0, 0, buffer->width, buffer->height,
                  buffer->memory,
                  &buffer->info,
                  DIB_RGB_COLORS, SRCCOPY);
}

static void Render(win32_offscreen_buffer *buffer)
{
    uint8_t *row = (uint8_t *)buffer->memory;
    for(int y = 0; y < buffer->height; ++y)
    {
        uint8_t *pixel = (uint8_t *)row;
        for (int x = 0; x < buffer->width; ++x)
        {
            *pixel = ((double)rand() / 255) * (255 - 0) + 0;
            ++pixel;
            
            *pixel = ((double)rand() / 255) * (255 - 0) + 0;
            ++pixel;
            
            *pixel = ((double)rand() / 255) * (255 - 0) + 0;
            ++pixel;
            
            *pixel = 0;
            ++pixel;
        }
        
        row += buffer->pitch;
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Win32LoadXInput();

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
    
    if (hwnd == NULL)
    {
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    HDC hdc = GetDC(hwnd);
    
    Running = true;
    while(Running)
    {
        MSG msg = {0};
        
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                Running = false;
            }
            
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        HandleInput(); 

        Render(&Backbuffer);
        
        win32_window_dimensions dimensions = GetWindowDimensions(hwnd);
        Win32CopyBufferToWindow(&Backbuffer, hdc, dimensions.width, dimensions.height);
    }
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_SIZE:
            {
            } break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
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
            {
                uint32_t VKCode = wParam;
                bool wasDown = ((lParam & (1 << 30)) != 0);
                bool isDown = ((lParam & (1 << 31)) == 0);

                if(wasDown != isDown)
                {
                    switch(VKCode) {
                        case 'W': 
                            {
                                OutputDebugStringA("W\n");
                            } break;
                        case 'A':
                            {
                                OutputDebugStringA("A\n");
                            } break;
                        case 'S':
                            {
                                OutputDebugStringA("S\n");
                            } break;
                        case 'D':
                            {
                                OutputDebugStringA("D\n");
                            } break;
                        case 'Q':
                            {
                            } break;
                        case 'E':
                            {
                            } break;
                        case VK_UP:
                            {
                            } break;
                        case VK_DOWN:
                            {
                            } break;
                        case VK_LEFT:
                            {
                            } break;
                        case VK_RIGHT:
                            {
                            } break;
                        case VK_ESCAPE:
                            {
                                OutputDebugStringA("ESCAPE: ");
                                if(isDown)
                                {
                                    OutputDebugStringA("IsDown ");
                                }
                                if(wasDown)
                                {
                                    OutputDebugStringA("WasDown");
                                }
                                OutputDebugStringA("\n");
                            } break;
                        case VK_SPACE:
                            {
                            } break;
                    }
                }
            } break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
