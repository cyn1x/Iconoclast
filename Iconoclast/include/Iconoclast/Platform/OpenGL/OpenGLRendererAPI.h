#pragma once

#include "OpenGLContext.h"
#include "RendererAPI.h"

namespace Iconoclast {

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        OpenGLRendererAPI();
        ~OpenGLRendererAPI();

        void             Initialize(GraphicsContext *) override;
        void             SetClearColor() override;
        void             Clear() override;
        void             BeginScene(Camera *camera) override;
        void             EndScene() override;
        void             Draw() override;
        void             Shutdown() override;
        GraphicsContext *GetContext() override;

    private:
        OpenGLContext *m_Context;
    };

} // namespace Iconoclast
