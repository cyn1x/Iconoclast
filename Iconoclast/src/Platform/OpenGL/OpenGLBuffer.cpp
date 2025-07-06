#include "GraphicsContext.h"
#include "IconoclastPCH.h"

#include "OpenGLBuffer.h"

namespace Iconoclast {

    OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex *vertices, uint32_t size)
    {
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
    }

    void OpenGLVertexBuffer::Initialize(float_t *vertices, uint32_t size)
    {
    }

    void OpenGLVertexBuffer::Shutdown()
    {
    }

    void OpenGLVertexBuffer::Bind(GraphicsContext &context) const
    {
    }

    void OpenGLVertexBuffer::Unbind(GraphicsContext &context) const
    {
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t size)
    {
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
    }

    void OpenGLIndexBuffer::Initialize(uint32_t *indices, uint32_t count)
    {
    }

    void OpenGLIndexBuffer::Shutdown()
    {
    }

    void OpenGLIndexBuffer::Bind(GraphicsContext &context) const
    {
    }

    void OpenGLIndexBuffer::Unbind(GraphicsContext &context) const
    {
    }

} // namespace Iconoclast
