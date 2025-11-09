#include "IconoclastPCH.h" // IWYU pragma: export

#include "OpenGLContext.h"

namespace Iconoclast {

    OpenGLContext::OpenGLContext(const ContextProps &props)
    {
        Initialize(props);
    }

    OpenGLContext::~OpenGLContext()
    {
    }

    void OpenGLContext::Initialize(const ContextProps &props)
    {
    }

    void OpenGLContext::SwapBuffers()
    {
    }

    void OpenGLContext::ShutDown()
    {
    }

} // namespace Iconoclast
