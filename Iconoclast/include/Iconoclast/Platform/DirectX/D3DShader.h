#pragma once

#include "GraphicsContext.h"
#include "Scene.h"
#include "Shader.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DShader : public Shader
    {
    private:
        struct MatrixBufferType
        {
            XMMATRIX world;
            XMMATRIX view;
            XMMATRIX projection;
        };

    public:
        D3DShader(GraphicsContext &context, const std::filesystem::path &vertexSrc,
                  const std::filesystem::path &pixelSrc);
        ~D3DShader();

        void Bind(GraphicsContext &context, const SceneData &sceneData) const override;
        void Unbind() const override;

        void Shutdown();

    private:
        HRESULT Initialize(ID3D11Device *, HWND, WCHAR *, WCHAR *);
        void    OutputShaderErrorMessage(ID3D10Blob *, HWND, WCHAR *);

    private:
        ID3D11VertexShader *m_VertexShader;
        ID3D11PixelShader  *m_PixelShader;
        ID3D11InputLayout  *m_Layout;
        ID3D11Buffer       *m_MatrixBuffer;
    };

} // namespace Iconoclast
