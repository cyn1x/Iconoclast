#pragma once

#include "Scene.h" // IWYU pragma: keep

namespace Iconoclast {

    class Camera
    {
    public:
        virtual ~Camera()
        {
        }

        virtual void   SetPosition(float, float, float) = 0;
        virtual void   SetRotation(float, float, float) = 0;
        virtual void   Render(SceneData *)              = 0;

        static Camera *Create();

    protected:
        float m_PositionX, m_PositionY, m_PositionZ;
        float m_RotationX, m_RotationY, m_RotationZ;
    };

} // namespace Iconoclast
