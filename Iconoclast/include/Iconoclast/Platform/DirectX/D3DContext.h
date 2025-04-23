#pragma once

#include "IconoclastPCH.h" // IWYU pragma: keep

#include "GraphicsContext.h"

namespace Iconoclast {

    class D3DContext : public GraphicsContext
    {
    public:
        D3DContext(const ContextProps &props);
        ~D3DContext();

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
        ID3D11Device           *GetDevice();
        ID3D11DeviceContext    *GetDeviceContext();
        ID3D11RenderTargetView *GetRenderTargetView();

        inline HWND             GetWindowHandle()
        {
            return m_WindowHandle;
        }

        void GetVideoCardInfo(char *cardName, int &memory);

    private:
        int                     m_VideoCardMemory;
        char                    m_VideoCardDescription[128];
        unsigned int            m_Width;
        unsigned int            m_Height;

        HWND                    m_WindowHandle;

        IDXGISwapChain         *m_SwapChain;
        ID3D11Device           *m_Device;
        ID3D11DeviceContext    *m_DeviceContext;
        ID3D11RenderTargetView *m_RenderTargetView;
    };

} // namespace Iconoclast
