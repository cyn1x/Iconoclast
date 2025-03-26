#include "IconoclastPCH.h" // IWYU pragma: keep

#include "D3DContext.h"

namespace Iconoclast

{
    GraphicsContext *GraphicsContext::Create(const ContextProps &props)
    {
        return new D3DContext(props);
    }

    D3DContext::D3DContext(const ContextProps &props)
    {
        Init(props);
    }

    D3DContext::~D3DContext()
    {
    }

    void D3DContext::Init(const ContextProps &props)
    {
        unsigned int                  width, height;
        HRESULT                       result;
        IDXGIFactory                 *factory;
        IDXGIAdapter                 *adapter;
        IDXGIOutput                  *adapterOutput;
        unsigned int                  numModes, numerator, denominator;
        unsigned long long            stringLength;
        DXGI_MODE_DESC               *displayModeList;
        DXGI_ADAPTER_DESC             adapterDesc;
        int                           error;
        DXGI_SWAP_CHAIN_DESC          swapChainDesc;
        D3D_FEATURE_LEVEL             featureLevel;
        ID3D11Texture2D              *backBufferPtr;
        D3D11_TEXTURE2D_DESC          depthBufferDesc;
        D3D11_DEPTH_STENCIL_DESC      depthStencilDesc;
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        D3D11_RASTERIZER_DESC         rasterDesc;
        float                         fieldOfView, screenAspect;

        numerator = denominator = 0;
        m_VSync                 = props.VSync;
        m_FullScreen            = props.FullScreen;

        // Get window dimensions
        HWND hwnd = GetActiveWindow();
        RECT windowDimensions;
        result = GetWindowRect(hwnd, &windowDimensions);
        width  = windowDimensions.right - windowDimensions.left;
        height = windowDimensions.bottom - windowDimensions.top;

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
            if (displayModeList[i].Width == (unsigned int)width) {
                if (displayModeList[i].Height == (unsigned int)height) {
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
        swapChainDesc.BufferDesc.Width  = width;
        swapChainDesc.BufferDesc.Height = height;

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

        // Initialize the description of the depth buffer.
        ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

        // Set up the description of the depth buffer.
        depthBufferDesc.Width              = width;
        depthBufferDesc.Height             = height;
        depthBufferDesc.MipLevels          = 1;
        depthBufferDesc.ArraySize          = 1;
        depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count   = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags     = 0;
        depthBufferDesc.MiscFlags          = 0;

        // Create the texture for the depth buffer using the filled out description.
        result = m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Initialize the description of the stencil state.
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

        // Set up the description of the stencil state.
        depthStencilDesc.DepthEnable      = true;
        depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;

        depthStencilDesc.StencilEnable    = true;
        depthStencilDesc.StencilReadMask  = 0xFF;
        depthStencilDesc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing.
        depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing.
        depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

        // Create the depth stencil state.
        result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Set the depth stencil state.
        m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

        // Initialize the depth stencil view.
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

        // Set up the depth stencil view description.
        depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        // Create the depth stencil view.
        result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc,
                                                  &m_DepthStencilView);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Bind the render target view and depth stencil buffer to the output render pipeline.
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

        // Setup the raster description which will determine how and what polygons will be drawn.
        rasterDesc.AntialiasedLineEnable = false;
        rasterDesc.CullMode              = D3D11_CULL_BACK;
        rasterDesc.DepthBias             = 0;
        rasterDesc.DepthBiasClamp        = 0.0f;
        rasterDesc.DepthClipEnable       = true;
        rasterDesc.FillMode              = D3D11_FILL_SOLID;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.MultisampleEnable     = false;
        rasterDesc.ScissorEnable         = false;
        rasterDesc.SlopeScaledDepthBias  = 0.0f;

        // Create the rasterizer state from the description we just filled out.
        result = m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Now set the rasterizer state.
        m_DeviceContext->RSSetState(m_RasterState);

        // Setup the viewport for rendering.
        m_Viewport.Width    = (float)width;
        m_Viewport.Height   = (float)height;
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0f;
        m_Viewport.TopLeftX = 0.0f;
        m_Viewport.TopLeftY = 0.0f;

        // Create the viewport.
        m_DeviceContext->RSSetViewports(1, &m_Viewport);

        // Setup the projection matrix.
        fieldOfView  = 3.141592654f / 4.0f;
        screenAspect = (float)width / (float)height;

        // Create the projection matrix for 3D rendering.
        m_ProjectionMatrix =
            XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_ScreenNear, m_ScreenDepth);

        // Initialize the world matrix to the identity matrix.
        m_WorldMatrix = XMMatrixIdentity();

        // Create an orthographic projection matrix for 2D rendering.
        m_OrthoMatrix =
            XMMatrixOrthographicLH((float)width, (float)height, m_ScreenNear, m_ScreenDepth);
    }

    void D3DContext::ShutDown()
    {
        // Before shutting down set to windowed mode or when you release the swap chain it will
        // throw an exception.
        if (m_SwapChain) {
            m_SwapChain->SetFullscreenState(false, NULL);
        }

        if (m_RasterState) {
            m_RasterState->Release();
            m_RasterState = 0;
        }

        if (m_DepthStencilView) {
            m_DepthStencilView->Release();
            m_DepthStencilView = 0;
        }

        if (m_DepthStencilState) {
            m_DepthStencilState->Release();
            m_DepthStencilState = 0;
        }

        if (m_DepthStencilBuffer) {
            m_DepthStencilBuffer->Release();
            m_DepthStencilBuffer = 0;
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

    void D3DContext::BeginScene()
    {
        // Setup the color to clear the buffer to.
        float color[4] = {0.1f, 0.3f, 0.3f, 1.0f};

        // Clear the back buffer.
        m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);

        // Clear the depth buffer.
        m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void D3DContext::EndScene()
    {
        SwapBuffers();
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

    ID3D11Device *D3DContext::GetDevice()
    {
        return m_Device;
    }

    ID3D11DeviceContext *D3DContext::GetDeviceContext()
    {
        return m_DeviceContext;
    }

    void D3DContext::GetProjectionMatrix(XMMATRIX &projectionMatrix)
    {
        projectionMatrix = m_ProjectionMatrix;
        return;
    }

    void D3DContext::GetWorldMatrix(XMMATRIX &worldMatrix)
    {
        worldMatrix = m_WorldMatrix;
        return;
    }

    void D3DContext::GetOrthoMatrix(XMMATRIX &orthoMatrix)
    {
        orthoMatrix = m_OrthoMatrix;
        return;
    }

    void D3DContext::GetVideoCardInfo(char *cardName, int &memory)
    {
        strcpy_s(cardName, 128, m_VideoCardDescription);
        memory = m_VideoCardMemory;
        return;
    }

    void D3DContext::SetBackBufferRenderTarget()
    {
        // Bind the render target view and depth stencil buffer to the output render pipeline.
        m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

        return;
    }

    void D3DContext::ResetViewport()
    {
        // Set the viewport.
        m_DeviceContext->RSSetViewports(1, &m_Viewport);

        return;
    }

} // namespace Iconoclast
