#pragma once

#include "IconoclastPCH.h" // IWYU pragma: keep

#include "GraphicsContext.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DContext : public GraphicsContext
    {
    public:
        D3DContext(const ContextProps &props);
        ~D3DContext();

        void BeginScene() override;
        void EndScene() override;
        void SwapBuffers() override;
        void ShutDown() override;

    public:
        ID3D11Device        *GetDevice();
        ID3D11DeviceContext *GetDeviceContext();

        void                 Init(const ContextProps &props);
        void                 GetProjectionMatrix(XMMATRIX &projectionMatrix);
        void                 GetWorldMatrix(XMMATRIX &worldMatrix);
        void                 GetOrthoMatrix(XMMATRIX &orthoMatrix);
        void                 GetVideoCardInfo(char *cardName, int &memory);
        void                 SetBackBufferRenderTarget();
        void                 ResetViewport();

    private:
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
