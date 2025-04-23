#pragma once

#include "IconoclastPCH.h"

#include "GraphicsContext.h"
#include "Scene.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4820)

namespace Iconoclast {

    using namespace DirectX;

    class Shader
    {
    public:
        virtual ~Shader()
        {
        }

        virtual void Bind(GraphicsContext &context, const SceneData &sceneData) const = 0;
        virtual void Unbind() const                                                   = 0;

    protected:
        std::filesystem::path m_VertexSrc;
        std::filesystem::path m_PixelSrc;
    };

} // namespace Iconoclast

#pragma warning(pop)
