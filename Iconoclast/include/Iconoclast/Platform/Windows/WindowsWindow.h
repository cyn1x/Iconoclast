#pragma once

#include "Window.h"

namespace Iconoclast {

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps &props);
        ~WindowsWindow();

        void                OnUpdate() override;

        inline unsigned int GetWidth() const override
        {
            return m_Data.Width;
        }
        inline unsigned int GetHeight() const override
        {
            return m_Data.Height;
        }

        inline void SetEventCallback(const EventCallbackFn &callback) override
        {
            m_Data.EventCallback = callback;
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        HWND GetWindowHandle()
        {
            return m_WindowHandle;
        };

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        DWORD                   Init(const WindowProps &props);
        void                    Shutdown();

    private:
        struct WindowData
        {
            std::string     Title;
            unsigned int    Width, Height;
            bool            VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
        HWND       m_WindowHandle;
    };

} // namespace Iconoclast
