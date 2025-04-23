#pragma once

#include "IconoclastPCH.h" // IWYU pragma: keep

#include "Camera.h"

namespace Iconoclast {

    using namespace DirectX;

    class D3DCamera : public Camera
    {
    public:
        D3DCamera();
        D3DCamera(D3DCamera &);
        ~D3DCamera();

        void     SetPosition(float, float, float) override;
        void     SetRotation(float, float, float) override;
        void     Render(SceneData *) override;

        XMFLOAT3 GetPosition();
        XMFLOAT3 GetRotation();

        void     GetViewMatrix(XMMATRIX &);

    private:
        XMMATRIX m_ViewMatrix;
    };

} // namespace Iconoclast
