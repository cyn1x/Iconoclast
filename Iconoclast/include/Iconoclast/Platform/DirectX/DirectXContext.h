#pragma once

#include "GraphicsContext.h"
#include <d3d11.h>
#include <directxmath.h>

namespace Iconoclast {

    class DirectXContext : public GraphicsContext
    {
    public:
        DirectXContext(HWND hwnd);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        HWND m_WindowHandle;
    };

} // namespace Iconoclast
