#pragma once

#include "RenderObject.h"

namespace Iconoclast {

    class OpenGLRenderObject : public RenderObject
    {
    public:
        OpenGLRenderObject(GraphicsContext &context);
        ~OpenGLRenderObject();

        void CreateVertexBuffer(Vertex *, uint32_t) override;
        void CreateIndexBuffer(uint32_t *, uint32_t) override;
        void CreateShader(std::string const, std::string const) override;
        void Render() override;
    };

} // namespace Iconoclast
