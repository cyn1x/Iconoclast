#include "IconoclastPCH.h" // IWYU pragma: keep

#include "D3DBuffer.h"
#include "D3DContext.h"
#include "D3DRenderObject.h"
#include "D3DShader.h"
#include "Scene.h"

namespace Iconoclast {

    D3DRenderObject::D3DRenderObject(GraphicsContext &context) : RenderObject(context)
    {
        m_VertexBuffer = 0;
        m_IndexBuffer  = 0;
    }

    D3DRenderObject::~D3DRenderObject()
    {
    }

    void D3DRenderObject::CreateVertexBuffer(Vertex *data, uint32_t size)
    {
        D3DContext *ctx = static_cast<D3DContext *>(m_Context);
        m_VertexBuffer  = std::make_unique<D3DVertexBuffer>(ctx->GetDevice(), data, size);
    }

    void D3DRenderObject::CreateIndexBuffer(uint32_t *data, uint32_t count)
    {
        D3DContext *ctx = static_cast<D3DContext *>(m_Context);
        m_IndexBuffer   = std::make_unique<D3DIndexBuffer>(ctx->GetDevice(), data, count);
    }

    void D3DRenderObject::CreateShader(const std::string src, const std::string dst)
    {
        D3DContext *ctx = static_cast<D3DContext *>(m_Context);
        m_Shader        = std::make_unique<D3DShader>(*m_Context, src, dst);
    }

    void D3DRenderObject::Render()
    {
        D3DContext      *ctx          = static_cast<D3DContext *>(m_Context);
        D3DShader       *shader       = static_cast<D3DShader *>(m_Shader.get());
        D3DVertexBuffer *vertexBuffer = static_cast<D3DVertexBuffer *>(m_VertexBuffer.get());
        D3DIndexBuffer  *indexBuffer  = static_cast<D3DIndexBuffer *>(m_IndexBuffer.get());

        // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
        vertexBuffer->Bind(*m_Context);
        indexBuffer->Bind(*m_Context);
        shader->Bind(*m_Context, *Scene::GetSceneData());

        // Render the shape.
        ctx->GetDeviceContext()->DrawIndexed(m_IndexBuffer->GetCount(), 0, 0);
    }

} // namespace Iconoclast
