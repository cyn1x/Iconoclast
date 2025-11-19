#pragma once

#include "GraphicsContext.h"
#include "OpenGLFunctions.h"

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
        HDC         GetDeviceContext();
        HGLRC       GetRenderingContext();

        inline HWND GetWindowHandle()
        {
            return m_WindowHandle;
        }

        void GetVideoCardInfo(char *cardName);

    private:
        bool InitializeExtensions(HWND hwnd);
        bool LoadExtensionList();

    private:
        int                               m_VideoCardMemory;
        char                              m_VideoCardDescription[128];
        unsigned int                      m_Width;
        unsigned int                      m_Height;

        HWND                              m_WindowHandle;

        HDC                               m_DeviceContext;
        HGLRC                             m_RenderingContext;

        PFNWGLCHOOSEPIXELFORMATARBPROC    wglChoosePixelFormatARB;
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
        PFNWGLSWAPINTERVALEXTPROC         wglSwapIntervalEXT;

    public:
        PFNGLATTACHSHADERPROC             glAttachShader;
        PFNGLBINDBUFFERPROC               glBindBuffer;
        PFNGLBINDVERTEXARRAYPROC          glBindVertexArray;
        PFNGLBUFFERDATAPROC               glBufferData;
        PFNGLCOMPILESHADERPROC            glCompileShader;
        PFNGLCREATEPROGRAMPROC            glCreateProgram;
        PFNGLCREATESHADERPROC             glCreateShader;
        PFNGLDELETEBUFFERSPROC            glDeleteBuffers;
        PFNGLDELETEPROGRAMPROC            glDeleteProgram;
        PFNGLDELETESHADERPROC             glDeleteShader;
        PFNGLDELETEVERTEXARRAYSPROC       glDeleteVertexArrays;
        PFNGLDETACHSHADERPROC             glDetachShader;
        PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
        PFNGLGENBUFFERSPROC               glGenBuffers;
        PFNGLGENVERTEXARRAYSPROC          glGenVertexArrays;
        PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
        PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
        PFNGLGETPROGRAMIVPROC             glGetProgramiv;
        PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
        PFNGLGETSHADERIVPROC              glGetShaderiv;
        PFNGLLINKPROGRAMPROC              glLinkProgram;
        PFNGLSHADERSOURCEPROC             glShaderSource;
        PFNGLUSEPROGRAMPROC               glUseProgram;
        PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
        PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
        PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
        PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
        PFNGLACTIVETEXTUREPROC            glActiveTexture;
        PFNGLUNIFORM1IPROC                glUniform1i;
        PFNGLGENERATEMIPMAPPROC           glGenerateMipmap;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
        PFNGLUNIFORM3FVPROC               glUniform3fv;
        PFNGLUNIFORM4FVPROC               glUniform4fv;
    };

} // namespace Iconoclast
