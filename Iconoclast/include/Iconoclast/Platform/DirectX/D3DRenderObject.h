#pragma once

#include "RenderObject.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DRenderObject : public RenderObject
    {
    private:
        struct VertexType
        {
            XMFLOAT3 position;
            XMFLOAT4 color;
        };

    public:
        D3DRenderObject(GraphicsContext &context);
        ~D3DRenderObject();

        void CreateVertexBuffer(Vertex *, uint32_t) override;
        void CreateIndexBuffer(uint32_t *, uint32_t) override;
        void CreateShader(std::string const, std::string const) override;
        void Render() override;
    };

} // namespace Iconoclast
