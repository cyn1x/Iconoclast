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
        void BuildIdentityMatrix(float *);
        void BuildPerspectiveFovLHMatrix(float *, float, float, float, float);
        void MatrixRotationY(float *matrix, float angle);
        void MatrixTranslation(float *matrix, float x, float y, float z);
        void MatrixMultiply(float *result, float *matrix1, float *matrix2);

    private:
        float          m_ScreenDepth = 1000.0f;
        float          m_ScreenNear  = 0.1f;

        float          m_WorldMatrix[16];
        float          m_ProjectionMatrix[16];

        OpenGLContext *m_Context;
    };

} // namespace Iconoclast
