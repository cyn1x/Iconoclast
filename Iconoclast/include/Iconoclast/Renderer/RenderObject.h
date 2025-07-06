#pragma once

#include "Buffer.h"
#include "GraphicsContext.h"
#include "Shader.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)

namespace Iconoclast {

    class RenderObject
    {
    public:
        virtual ~RenderObject()
        {
            m_Context = nullptr;
        }
        RenderObject()                                                   = delete;
        RenderObject(const RenderObject &)                               = delete;
        RenderObject &operator=(const RenderObject &)                    = delete;

        virtual void  CreateVertexBuffer(Vertex *, uint32_t)             = 0;
        virtual void  CreateIndexBuffer(uint32_t *, uint32_t)            = 0;
        virtual void  CreateShader(std::string const, std::string const) = 0;
        virtual void  Render()                                           = 0;

        static std::function<RenderObject *(GraphicsContext &)> Create;

    protected:
        explicit RenderObject(GraphicsContext &context) : m_Context(&context)
        {
        }

        GraphicsContext              *m_Context;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer>  m_IndexBuffer;
        std::unique_ptr<Shader>       m_Shader;
    };

} // namespace Iconoclast

#pragma warning(pop)
