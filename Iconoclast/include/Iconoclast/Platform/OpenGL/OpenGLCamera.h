#pragma once

#include "IconoclastPCH.h" // IWYU pragma: keep

#include "Camera.h"

namespace Iconoclast {

    class OpenGLCamera : public Camera
    {
    public:
        OpenGLCamera();
        OpenGLCamera(OpenGLCamera &);
        ~OpenGLCamera();

        void SetPosition(float, float, float) override;
        void SetRotation(float, float, float) override;
        void Render(SceneData *) override;
    };

} // namespace Iconoclast
