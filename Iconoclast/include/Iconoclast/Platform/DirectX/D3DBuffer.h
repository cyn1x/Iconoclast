#pragma once

#include "Buffer.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DVertexBuffer : public VertexBuffer
    {
    private:
        struct VertexType
        {
            XMFLOAT3 position;
            XMFLOAT4 color;
        };

    public:
        D3DVertexBuffer(ID3D11Device *, Vertex *, uint32_t);
        ~D3DVertexBuffer();

        void Bind(GraphicsContext &) const override;
        void Unbind(GraphicsContext &) const override;

    private:
        void          Initialize(ID3D11Device *, Vertex *, uint32_t);
        void          Shutdown();

        ID3D11Buffer *m_VertexBuffer;
        uint32_t      m_VertexCount;
    };

    class D3DIndexBuffer : public IndexBuffer
    {
    public:
        D3DIndexBuffer(ID3D11Device *, uint32_t *, uint32_t);
        ~D3DIndexBuffer();

        void     Bind(GraphicsContext &) const override;
        void     Unbind(GraphicsContext &) const override;

        uint32_t GetCount() const override
        {
            return m_IndexCount;
        }

    private:
        void          Initialize(ID3D11Device *, uint32_t *);
        void          Shutdown();

        ID3D11Buffer *m_IndexBuffer;
        uint32_t      m_IndexCount;
    };

} // namespace Iconoclast
