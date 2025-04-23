#pragma once

#include "Buffer.h"
#include "GraphicsContext.h"
#include "Shader.h"

namespace Iconoclast {

    class Mesh
    {
    public:
        virtual ~Mesh()
        {
            m_Context = nullptr;
        }
        Mesh()                                                                  = default;
        Mesh(const Mesh &)                                                      = delete;
        Mesh                                           &operator=(const Mesh &) = delete;

        virtual void                                    CreateVertexBuffer(Vertex *, uint32_t)  = 0;
        virtual void                                    CreateIndexBuffer(uint32_t *, uint32_t) = 0;
        virtual void                                    CreateShader(std::string, std::string)  = 0;
        virtual void                                    Render()                                = 0;

        static std::function<Mesh *(GraphicsContext &)> Create;

    protected:
        GraphicsContext              *m_Context;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer>  m_IndexBuffer;
        std::unique_ptr<Shader>       m_Shader;
    };

} // namespace Iconoclast
