#pragma once

#include "GraphicsContext.h"
#include "Scene.h"
#include "Shader.h"

namespace Iconoclast {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(GraphicsContext &context, const std::filesystem::path &vertexSrc,
                     const std::filesystem::path &pixelSrc);
        ~OpenGLShader();

        void Bind(GraphicsContext &context, const SceneData &sceneData) const override;
        void Unbind() const override;
    };

} // namespace Iconoclast
