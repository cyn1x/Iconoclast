#pragma once

#include "D3DContext.h"
#include "RendererAPI.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DRendererAPI : public RendererAPI
    {
    public:
        D3DRendererAPI();
        ~D3DRendererAPI();

        void             Initialize(GraphicsContext *) override;
        void             SetClearColor() override;
        void             Clear() override;
        void             BeginScene(Camera *camera) override;
        void             EndScene() override;
        void             Draw() override;
        void             Shutdown() override;
        GraphicsContext *GetContext() override;

    public:
        void GetProjectionMatrix(XMMATRIX &projectionMatrix);
        void GetWorldMatrix(XMMATRIX &worldMatrix);
        void GetOrthoMatrix(XMMATRIX &orthoMatrix);
        void SetBackBufferRenderTarget();
        void ResetViewport();

    private:
        float                    m_ScreenDepth = 1000.0f;
        float                    m_ScreenNear  = 0.3f;

        D3DContext              *m_Context;

        ID3D11Texture2D         *m_DepthStencilBuffer;
        ID3D11DepthStencilState *m_DepthStencilState;
        ID3D11DepthStencilView  *m_DepthStencilView;
        ID3D11RasterizerState   *m_RasterState;
        XMMATRIX                 m_ProjectionMatrix;
        XMMATRIX                 m_WorldMatrix;
        XMMATRIX                 m_OrthoMatrix;
        D3D11_VIEWPORT           m_Viewport;
    };

} // namespace Iconoclast
