
#define ICONOCLAST_EXPORTS

#include "Window.h"
#include "WindowsWindow.h"

namespace Iconoclast {
    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC         hdc = BeginPaint(hwnd, &ps);

                // All painting occurs here, between BeginPaint and EndPaint.

                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

                EndPaint(hwnd, &ps);
            }
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    Window *Window::Create(const WindowProps &props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
    }

    DWORD WindowsWindow::Init(const WindowProps &props)
    {
        m_Data.Title  = props.Title;
        m_Data.Width  = props.Width;
        m_Data.Height = props.Height;

#ifdef _WIN64
        char *dll = (char *)"iconoclast_x64.dll";
#elif _WIN32
        char *dll = (char *)"iconoclast_x86.dll";
#endif

        HINSTANCE hInstance = GetModuleHandleA(dll);

        // Register the window class.
        const wchar_t CLASS_NAME[] = L"Sample Window Class";

        WNDCLASS      wc           = {};

        wc.lpfnWndProc             = WindowsWindow::WindowProc;
        wc.hInstance               = hInstance;
        wc.lpszClassName           = CLASS_NAME;

        RegisterClass(&wc);

        // Create the window.

        HWND hwnd = CreateWindowEx(0,                           // Optional window styles.
                                   CLASS_NAME,                  // Window class
                                   L"Learn to Program Windows", // Window text
                                   WS_OVERLAPPEDWINDOW,         // Window style

                                   // Size and position
                                   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

                                   NULL,      // Parent window
                                   NULL,      // Menu
                                   hInstance, // Instance handle
                                   this       // Additional application data
        );

        if (hwnd == NULL) {
            // TODO: Logging
            return 0;
        }

        ShowWindow(hwnd, 1);

        return 0;
    }

    void WindowsWindow::Shutdown()
    {
    }

    void WindowsWindow::OnUpdate()
    {
        // Run the message loop.

        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
            if (msg.message == WM_QUIT) {
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

} // namespace Iconoclast
