#pragma once

#include "IconoclastPCH.h" // IWYU pragma: keep

#include "GraphicsContext.h"

namespace Iconoclast {

    using namespace DirectX;

    class DirectXContext : public GraphicsContext
    {
    public:
        DirectXContext(const ContextProps &props);
        virtual ~DirectXContext();

        virtual void SwapBuffers() override;
        virtual void ShutDown() override;

    private:
        void                     Init(const ContextProps &props);

        float                    m_ScreenDepth = 1000.0f;
        float                    m_ScreenNear  = 0.3f;

        int                      m_VideoCardMemory;
        char                     m_VideoCardDescription[128];
        IDXGISwapChain          *m_SwapChain;
        ID3D11Device            *m_Device;
        ID3D11DeviceContext     *m_DeviceContext;
        ID3D11RenderTargetView  *m_RenderTargetView;
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
