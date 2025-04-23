#include "IconoclastPCH.h" // IWYU pragma: keep

#include "D3DContext.h"

namespace Iconoclast {

    D3DContext::D3DContext(const ContextProps &props)
    {
        Initialize(props);
    }

    D3DContext::~D3DContext()
    {
    }

    void D3DContext::Initialize(const ContextProps &props)
    {
        HRESULT              result;
        IDXGIFactory        *factory;
        IDXGIAdapter        *adapter;
        IDXGIOutput         *adapterOutput;
        unsigned int         numModes, numerator, denominator;
        unsigned long long   stringLength;
        DXGI_MODE_DESC      *displayModeList;
        DXGI_ADAPTER_DESC    adapterDesc;
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        D3D_FEATURE_LEVEL    featureLevel;
        ID3D11Texture2D     *backBufferPtr;
        int                  error;

        numerator = denominator = 0;
        m_VSync                 = props.VSync;
        m_FullScreen            = props.FullScreen;

        //
        HWND hwnd      = GetActiveWindow();
        m_WindowHandle = hwnd;

        // Get window dimensions
        RECT windowDimensions;
        result   = GetWindowRect(hwnd, &windowDimensions);
        m_Width  = windowDimensions.right - windowDimensions.left;
        m_Height = windowDimensions.bottom - windowDimensions.top;

        // Store the vsync setting.
        m_VSync = props.VSync;

        // Create a DirectX graphics interface factory.
        result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Use the factory to create an adapter for the primary graphics interface (video card).
        result = factory->EnumAdapters(0, &adapter);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Enumerate the primary adapter output (monitor).
        result = adapter->EnumOutputs(0, &adapterOutput);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the
        // adapter output (monitor).
        result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                                   DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Create a list to hold all the possible display modes for this monitor/video card
        // combination.
        displayModeList = new DXGI_MODE_DESC[numModes];
        if (!displayModeList) {
            // TODO: Handle failure
        }

        // Now fill the display mode list structures.
        result = adapterOutput->GetDisplayModeList(
            DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Now go through all the display modes and find the one that matches the screen width and
        // height. When a match is found store the numerator and denominator of the refresh rate for
        // that monitor.
        for (unsigned int i = 0; i < numModes; i++) {
            if (displayModeList[i].Width == (unsigned int)m_Width) {
                if (displayModeList[i].Height == (unsigned int)m_Height) {
                    numerator   = displayModeList[i].RefreshRate.Numerator;
                    denominator = displayModeList[i].RefreshRate.Denominator;
                }
            }
        }

        // Get the adapter (video card) description.
        result = adapter->GetDesc(&adapterDesc);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Store the dedicated video card memory in megabytes.
        m_VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

        // Convert the name of the video card to a character array and store it.
        error =
            wcstombs_s(&stringLength, m_VideoCardDescription, 128, adapterDesc.Description, 128);
        if (error != 0) {
            // TODO: Handle failure
        }

        // Release the display mode list.
        delete[] displayModeList;
        displayModeList = 0;

        // Release the adapter output.
        adapterOutput->Release();
        adapterOutput = 0;

        // Release the adapter.
        adapter->Release();
        adapter = 0;

        // Release the factory.
        factory->Release();
        factory = 0;

        // Initialize the swap chain description.
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

        // Set to a single back buffer.
        swapChainDesc.BufferCount = 1;

        // Set the width and height of the back buffer.
        swapChainDesc.BufferDesc.Width  = m_Width;
        swapChainDesc.BufferDesc.Height = m_Height;

        // Set regular 32-bit surface for the back buffer.
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // Set the refresh rate of the back buffer.
        if (m_VSync) {
            swapChainDesc.BufferDesc.RefreshRate.Numerator   = numerator;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
        } else {
            swapChainDesc.BufferDesc.RefreshRate.Numerator   = 0;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        }

        // Set the usage of the back buffer.
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        // Set the handle for the window to render to.
        swapChainDesc.OutputWindow = hwnd;

        // Turn multisampling off.
        swapChainDesc.SampleDesc.Count   = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        // Set to full screen or windowed mode.
        if (m_FullScreen) {
            swapChainDesc.Windowed = false;
        } else {
            swapChainDesc.Windowed = true;
        }

        // Set the scan line ordering and scaling to unspecified.
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

        // Discard the back buffer contents after presenting.
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        // Don't set the advanced flags.
        swapChainDesc.Flags = 0;

        // Set the feature level to DirectX 11.
        featureLevel = D3D_FEATURE_LEVEL_11_0;

        // Create the swap chain, Direct3D device, and Direct3D device context.
        result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
                                               &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc,
                                               &m_SwapChain, &m_Device, NULL, &m_DeviceContext);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Get the pointer to the back buffer.
        result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&backBufferPtr);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Create the render target view with the back buffer pointer.
        result = m_Device->CreateRenderTargetView(backBufferPtr, NULL, &m_RenderTargetView);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Release pointer to the back buffer as we no longer need it.
        backBufferPtr->Release();
        backBufferPtr = 0;
    }

    void D3DContext::SwapBuffers()
    {
        // Present the back buffer to the screen since rendering is complete.
        if (m_VSync) {
            // Lock to screen refresh rate.
            m_SwapChain->Present(1, 0);
        } else {
            // Present as fast as possible.
            m_SwapChain->Present(0, 0);
        }
    }

    void D3DContext::ShutDown()
    {
        // Before shutting down set to windowed mode or when you release the swap chain it will
        // throw an exception.
        if (m_SwapChain) {
            m_SwapChain->SetFullscreenState(false, NULL);
        }

        if (m_RenderTargetView) {
            m_RenderTargetView->Release();
            m_RenderTargetView = 0;
        }

        if (m_DeviceContext) {
            m_DeviceContext->Release();
            m_DeviceContext = 0;
        }

        if (m_Device) {
            m_Device->Release();
            m_Device = 0;
        }

        if (m_SwapChain) {
            m_SwapChain->Release();
            m_SwapChain = 0;
        }
    }

    ID3D11RenderTargetView *D3DContext::GetRenderTargetView()
    {
        return m_RenderTargetView;
    }

    ID3D11Device *D3DContext::GetDevice()
    {
        return m_Device;
    }

    ID3D11DeviceContext *D3DContext::GetDeviceContext()
    {
        return m_DeviceContext;
    }

    void D3DContext::GetVideoCardInfo(char *cardName, int &memory)
    {
        strcpy_s(cardName, 128, m_VideoCardDescription);
        memory = m_VideoCardMemory;
    }

} // namespace Iconoclast
