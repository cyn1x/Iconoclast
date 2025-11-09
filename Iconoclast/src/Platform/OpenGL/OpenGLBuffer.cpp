#include "IconoclastPCH.h"

#include "OpenGLBuffer.h"

namespace Iconoclast {

    OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex *vertices, uint32_t size)
    {
        m_VertexBuffer = 0;
        m_VertexCount  = 0;

        Initialize(vertices, size);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        OpenGLVertexBuffer::Shutdown();
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t size)
    {
        m_IndexBuffer = 0;
        m_IndexCount  = 0;

        Initialize(indices, size);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        OpenGLIndexBuffer::Shutdown();
    }

    void OpenGLVertexBuffer::Initialize(Vertex *data, uint32_t size)
    {
    }

    void OpenGLIndexBuffer::Initialize(uint32_t *data, uint32_t count)
    {
    }

    void OpenGLVertexBuffer::Bind(GraphicsContext &context) const
    {
    }

    void OpenGLVertexBuffer::Unbind(GraphicsContext &context) const
    {
    }

    void OpenGLIndexBuffer::Bind(GraphicsContext &context) const
    {
    }

    void OpenGLIndexBuffer::Unbind(GraphicsContext &context) const
    {
    }

    void OpenGLVertexBuffer::Shutdown()
    {
    }

    void OpenGLIndexBuffer::Shutdown()
    {
    }

} // namespace Iconoclast
