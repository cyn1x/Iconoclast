#pragma once

#include "Buffer.h"

namespace Iconoclast {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(Vertex *, uint32_t);
        ~OpenGLVertexBuffer();

        void Bind(GraphicsContext &) const override;
        void Unbind(GraphicsContext &) const override;

    private:
        void     Initialize(Vertex *, uint32_t);
        void     Shutdown();

        float_t *m_VertexBuffer;
        uint32_t m_VertexCount;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t *, uint32_t);
        ~OpenGLIndexBuffer();

        void     Bind(GraphicsContext &) const override;
        void     Unbind(GraphicsContext &) const override;

        uint32_t GetCount() const override
        {
            return m_IndexCount;
        }

    private:
        void      Initialize(uint32_t *, uint32_t);
        void      Shutdown();

        uint32_t *m_IndexBuffer;
        uint32_t  m_IndexCount;
    };

} // namespace Iconoclast
