#include "IconoclastPCH.h"

#include "Camera.h"
#include "D3DCamera.h"

namespace Iconoclast {

    static Camera *CreateStub()
    {
        // TODO: Handle logging for when function is not assigned
        return nullptr;
    }

    Camera *CreateD3DCamera()
    {
        return new D3DCamera;
    }

    // TODO: Bind once on init for corresponding graphics platform
    // std::function<Mesh *(GraphicsContext &)> Mesh::Create = CreateStub;
    std::function<Camera *()> Camera::Create = CreateD3DCamera;

} // namespace Iconoclast
