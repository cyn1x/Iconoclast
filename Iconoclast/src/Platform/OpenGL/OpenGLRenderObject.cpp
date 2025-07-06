#include "IconoclastPCH.h" // IWYU pragma: keep

#include "OpenGLBuffer.h"
#include "OpenGLContext.h"
#include "OpenGLRenderObject.h"
#include "OpenGLShader.h"
#include "Scene.h"

namespace Iconoclast {

    OpenGLRenderObject::OpenGLRenderObject(GraphicsContext &context) : RenderObject(context)
    {
        m_VertexBuffer = 0;
        m_IndexBuffer  = 0;
    }

    OpenGLRenderObject::~OpenGLRenderObject()
    {
    }

    void OpenGLRenderObject::CreateVertexBuffer(Vertex *data, uint32_t size)
    {
        OpenGLContext *ctx = static_cast<OpenGLContext *>(m_Context);
        m_VertexBuffer     = std::make_unique<OpenGLVertexBuffer>(data, size);
    }

    void OpenGLRenderObject::CreateIndexBuffer(uint32_t *data, uint32_t count)
    {
        OpenGLContext *ctx = static_cast<OpenGLContext *>(m_Context);
        m_IndexBuffer      = std::make_unique<OpenGLIndexBuffer>(data, count);
    }

    void OpenGLRenderObject::CreateShader(const std::string src, const std::string dst)
    {
        OpenGLContext *ctx = static_cast<OpenGLContext *>(m_Context);
        m_Shader           = std::make_unique<OpenGLShader>(*m_Context, src, dst);
    }

    void OpenGLRenderObject::Render()
    {
        OpenGLContext      *ctx          = static_cast<OpenGLContext *>(m_Context);
        OpenGLShader       *shader       = static_cast<OpenGLShader *>(m_Shader.get());
        OpenGLVertexBuffer *vertexBuffer = static_cast<OpenGLVertexBuffer *>(m_VertexBuffer.get());
        OpenGLIndexBuffer  *indexBuffer  = static_cast<OpenGLIndexBuffer *>(m_IndexBuffer.get());

        // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
        vertexBuffer->Bind(*m_Context);
        indexBuffer->Bind(*m_Context);
        shader->Bind(*m_Context, *Scene::GetSceneData());

        // Render the shape.
        // TODO: Render the shape
    }

} // namespace Iconoclast
