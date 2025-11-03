#include "IconoclastPCH.h"

#include "D3DBuffer.h"
#include "D3DContext.h"

namespace Iconoclast {

    D3DVertexBuffer::D3DVertexBuffer(ID3D11Device *device, Vertex *vertices, uint32_t size)
    {
        m_VertexBuffer = 0;
        m_VertexCount  = 0;

        Initialize(device, vertices, size);
    }

    D3DVertexBuffer::~D3DVertexBuffer()
    {
        D3DVertexBuffer::Shutdown();
    }

    D3DIndexBuffer::D3DIndexBuffer(ID3D11Device *device, uint32_t *indices, uint32_t count)
        : m_IndexCount(count)
    {
        m_IndexBuffer = 0;
        m_IndexCount  = 0;

        Initialize(device, indices, count);
    }

    D3DIndexBuffer::~D3DIndexBuffer()
    {
        D3DIndexBuffer::Shutdown();
    }

    void D3DVertexBuffer::Initialize(ID3D11Device *device, Vertex *data, uint32_t size)
    {
        VertexType            *vertices;
        D3D11_BUFFER_DESC      vertexBufferDesc;
        D3D11_SUBRESOURCE_DATA vertexData;
        HRESULT                result;

        // Set the number of vertices in the vertex array.
        m_VertexCount = size;

        // Create the vertex array.
        vertices = new VertexType[m_VertexCount];
        if (!vertices) {
            // TODO: Log failure
        }

        // Load the vertex array with data.
        for (uint32_t i = 0; i < m_VertexCount; ++i) {
            vertices[i].position = XMFLOAT3(data[i].x, data[i].y, data[i].z); // Bottom left.
            vertices[i].color =
                XMFLOAT4(data[i].color.r, data[i].color.g, data[i].color.b, data[i].color.a);
        }

        // Set up the description of the static vertex buffer.
        vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth           = sizeof(VertexType) * m_VertexCount;
        vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags      = 0;
        vertexBufferDesc.MiscFlags           = 0;
        vertexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the vertex data.
        vertexData.pSysMem          = vertices;
        vertexData.SysMemPitch      = 0;
        vertexData.SysMemSlicePitch = 0;

        // Now create the vertex buffer.
        result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
        if (FAILED(result)) {
            // TODO: Log failure
        }

        // Release the arrays now that the vertex and index buffers have been created and loaded.
        delete[] vertices;
        vertices = 0;
    }

    void D3DIndexBuffer::Initialize(ID3D11Device *device, uint32_t *data, uint32_t count)
    {
        unsigned long         *indices;
        D3D11_BUFFER_DESC      indexBufferDesc;
        D3D11_SUBRESOURCE_DATA indexData;
        HRESULT                result;

        // Set the number of indices in the index array.
        m_IndexCount = count;

        // Create the index array.
        indices = new unsigned long[m_IndexCount];
        if (!indices) {
            // TODO: Log failure
        }

        // Load the index array with data.
        for (uint32_t i = 0; i < m_IndexCount; ++i) {
            indices[i] = data[i]; // Bottom left.
        }

        // Set up the description of the static index buffer.
        indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_IndexCount;
        indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags      = 0;
        indexBufferDesc.MiscFlags           = 0;
        indexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the index data.
        indexData.pSysMem          = indices;
        indexData.SysMemPitch      = 0;
        indexData.SysMemSlicePitch = 0;

        // Create the index buffer.
        result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
        if (FAILED(result)) {
            // TODO: Log failure
        }

        delete[] indices;
        indices = 0;
    }

    void D3DVertexBuffer::Bind(GraphicsContext &context) const
    {
        unsigned int stride;
        unsigned int offset;

        // Set vertex buffer stride and offset.
        stride          = sizeof(VertexType);
        offset          = 0;

        D3DContext *ctx = static_cast<D3DContext *>(&context);

        // Set the vertex buffer to active in the input assembler so it can be rendered.
        ctx->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    }

    void D3DVertexBuffer::Unbind(GraphicsContext &context) const
    {
        ID3D11Buffer *nullBuffer = nullptr;
        unsigned int  stride     = 0;
        unsigned int  offset     = 0;

        D3DContext   *ctx        = static_cast<D3DContext *>(&context);

        ctx->GetDeviceContext()->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
    }

    void D3DIndexBuffer::Bind(GraphicsContext &context) const
    {
        D3DContext *ctx = static_cast<D3DContext *>(&context);

        // Set the index buffer to active in the input assembler so it can be rendered.
        ctx->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Set the type of primitive that should be rendered from this vertex buffer, in this case
        // triangles.
        ctx->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    void D3DIndexBuffer::Unbind(GraphicsContext &context) const
    {
        D3DContext *ctx = static_cast<D3DContext *>(&context);
        ctx->GetDeviceContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
    }

    void D3DVertexBuffer::Shutdown()
    {
        // Release the vertex buffer.
        if (m_VertexBuffer) {
            m_VertexBuffer->Release();
            m_VertexBuffer = 0;
        }
    }

    void D3DIndexBuffer::Shutdown()
    {
        // Release the index buffer.
        if (m_IndexBuffer) {
            m_IndexBuffer->Release();
            m_IndexBuffer = 0;
        }
    }

} // namespace Iconoclast
