#include "IconoclastPCH.h" // IWYU pragma: keep

#include "D3DRendererAPI.h"

namespace Iconoclast {

    D3DRendererAPI::D3DRendererAPI()
    {
    }

    D3DRendererAPI::~D3DRendererAPI()
    {
    }

    void D3DRendererAPI::Initialize(GraphicsContext *context)
    {
        HRESULT                       result;
        D3D11_TEXTURE2D_DESC          depthBufferDesc;
        D3D11_DEPTH_STENCIL_DESC      depthStencilDesc;
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        D3D11_RASTERIZER_DESC         rasterDesc;
        float                         fieldOfView, screenAspect;

        // Get the device context for this window.
        m_Context = dynamic_cast<D3DContext *>(context);

        // Initialize the description of the depth buffer.
        ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

        // Set up the description of the depth buffer.
        depthBufferDesc.Width              = m_Context->GetWidth();
        depthBufferDesc.Height             = m_Context->GetHeight();
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
        result =
            m_Context->GetDevice()->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
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
        result = m_Context->GetDevice()->CreateDepthStencilState(&depthStencilDesc,
                                                                 &m_DepthStencilState);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Set the depth stencil state.
        m_Context->GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, 1);

        // Initialize the depth stencil view.
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

        // Set up the depth stencil view description.
        depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        // Create the depth stencil view.
        result = m_Context->GetDevice()->CreateDepthStencilView(
            m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Bind the render target view and depth stencil buffer to the output render pipeline.
        ID3D11RenderTargetView *renderTargetView = m_Context->GetRenderTargetView();
        m_Context->GetDeviceContext()->OMSetRenderTargets(1, &renderTargetView, m_DepthStencilView);

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
        result = m_Context->GetDevice()->CreateRasterizerState(&rasterDesc, &m_RasterState);
        if (FAILED(result)) {
            // TODO: Handle failure
        }

        // Now set the rasterizer state.
        m_Context->GetDeviceContext()->RSSetState(m_RasterState);

        // Setup the viewport for rendering.
        m_Viewport.Width    = (float)m_Context->GetWidth();
        m_Viewport.Height   = (float)m_Context->GetHeight();
        m_Viewport.MinDepth = 0.0f;
        m_Viewport.MaxDepth = 1.0f;
        m_Viewport.TopLeftX = 0.0f;
        m_Viewport.TopLeftY = 0.0f;

        // Create the viewport.
        m_Context->GetDeviceContext()->RSSetViewports(1, &m_Viewport);

        // Setup the projection matrix.
        fieldOfView  = 3.141592654f / 4.0f;
        screenAspect = (float)m_Context->GetWidth() / (float)m_Context->GetHeight();

        // Create the projection matrix for 3D rendering.
        m_ProjectionMatrix =
            XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_ScreenNear, m_ScreenDepth);

        // Initialize the world matrix to the identity matrix.
        m_WorldMatrix = XMMatrixIdentity();

        // Create an orthographic projection matrix for 2D rendering.
        m_OrthoMatrix =
            XMMatrixOrthographicLH((float)m_Context->GetWidth(), (float)m_Context->GetHeight(),
                                   m_ScreenNear, m_ScreenDepth);
    }

    void D3DRendererAPI::SetClearColor()
    {
    }

    void D3DRendererAPI::Clear()
    {
    }

    void D3DRendererAPI::BeginScene(Camera *camera)
    {
        // Setup the color to clear the buffer to.
        float color[4] = {0.1f, 0.3f, 0.3f, 1.0f};

        // Clear the back buffer.
        m_Context->GetDeviceContext()->ClearRenderTargetView(m_Context->GetRenderTargetView(),
                                                             color);

        // Clear the depth buffer.
        m_Context->GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH,
                                                             1.0f, 0);

        SceneData *sceneData = Scene::GetSceneData();

        // Generate the view matrix based on the camera's position.
        camera->Render(sceneData);

        // sceneData->worldMatrix      = m_WorldMatrix;
        // sceneData->projectionMatrix = m_ProjectionMatrix;
    }

    void D3DRendererAPI::EndScene()
    {
        m_Context->SwapBuffers();
    }

    void D3DRendererAPI::Draw()
    {
    }

    void D3DRendererAPI::Shutdown()
    {
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
    }

    void D3DRendererAPI::GetProjectionMatrix(XMMATRIX &projectionMatrix)
    {
        projectionMatrix = m_ProjectionMatrix;
    }

    void D3DRendererAPI::GetWorldMatrix(XMMATRIX &worldMatrix)
    {
        worldMatrix = m_WorldMatrix;
    }

    void D3DRendererAPI::GetOrthoMatrix(XMMATRIX &orthoMatrix)
    {
        orthoMatrix = m_OrthoMatrix;
    }

    void D3DRendererAPI::SetBackBufferRenderTarget()
    {
        // Bind the render target view and depth stencil buffer to the output render pipeline.
        ID3D11RenderTargetView *renderTargetView = m_Context->GetRenderTargetView();
        m_Context->GetDeviceContext()->OMSetRenderTargets(1, &renderTargetView, m_DepthStencilView);
    }

    void D3DRendererAPI::ResetViewport()
    {
        // Set the viewport.
        m_Context->GetDeviceContext()->RSSetViewports(1, &m_Viewport);
    }

    GraphicsContext *D3DRendererAPI::GetContext()
    {
        return m_Context;
    }

} // namespace Iconoclast
