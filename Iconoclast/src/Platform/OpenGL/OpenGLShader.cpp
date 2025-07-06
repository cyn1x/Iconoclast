#include "IconoclastPCH.h"

#include "OpenGLShader.h"

namespace Iconoclast {

    OpenGLShader::OpenGLShader(GraphicsContext &context, const std::filesystem::path &vertexSrc,
                               const std::filesystem::path &pixelSrc)
    {
    }

    OpenGLShader::~OpenGLShader()
    {
    }

    void OpenGLShader::Bind(GraphicsContext &context, const SceneData &sceneData) const
    {
    }

    void OpenGLShader::Unbind() const
    {
    }

} // namespace Iconoclast
