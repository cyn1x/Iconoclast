#include "IconoclastPCH.h"

#define ICONOCLAST_EXPORTS

#include "ApplicationEvent.h"
#include "Window.h"
#include "WindowsWindow.h"

namespace Iconoclast {
    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
    }

    Window *Window::Create(const WindowProps &props)
    {
        return new WindowsWindow(props);
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
                                   CW_USEDEFAULT, CW_USEDEFAULT, m_Data.Width, m_Data.Height,

                                   NULL,      // Parent window
                                   NULL,      // Menu
                                   hInstance, // Instance handle
                                   this       // Additional application data
        );

        if (hwnd == NULL) {
            // TODO: Logging
            return 0;
        }

        m_WindowHandle = hwnd;

        ShowWindow(hwnd, 1);

        return 0;
    }

    void WindowsWindow::OnUpdate()
    {
        // Run the message loop.

        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
            if (msg.message == WM_QUIT) {
                WindowCloseEvent event;
                m_Data.EventCallback(event);
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void WindowsWindow::Shutdown()
    {
    }

    LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

} // namespace Iconoclast
