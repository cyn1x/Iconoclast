#pragma once

#include "Mesh.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DMesh : public Mesh
    {
    private:
        struct VertexType
        {
            XMFLOAT3 position;
            XMFLOAT4 color;
        };

    public:
        D3DMesh(GraphicsContext &context);
        ~D3DMesh();

        void CreateVertexBuffer(Vertex *, uint32_t) override;
        void CreateIndexBuffer(uint32_t *, uint32_t) override;
        void CreateShader(std::string, std::string) override;
        void Render() override;
    };

} // namespace Iconoclast
