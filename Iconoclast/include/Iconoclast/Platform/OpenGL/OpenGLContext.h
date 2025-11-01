#pragma once

#include "IconoclastPCH.h"

#include <glad/glad.h>

#include "GraphicsContext.h"

namespace Iconoclast {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(const ContextProps &props);
        ~OpenGLContext();

        void Initialize(const ContextProps &) override;
        void SwapBuffers() override;
        void ShutDown() override;

    public:
        inline unsigned int GetWidth() const override
        {
            return m_Width;
        }
        inline unsigned int GetHeight() const override
        {
            return m_Height;
        }

    public:
        inline HWND GetWindowHandle()
        {
            return m_WindowHandle;
        }

        void GetVideoCardInfo(char *cardName, int &memory);

    private:
        int          m_VideoCardMemory;
        char         m_VideoCardDescription[128];
        unsigned int m_Width;
        unsigned int m_Height;

        HWND         m_WindowHandle;
    };

} // namespace Iconoclast
