#include "IconoclastPCH.h"

#include "Camera.h"
#include "D3DCamera.h"
#include "OpenGLCamera.h"

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

    Camera *CreateOpenGLCamera()
    {
        return new OpenGLCamera;
    }

    /*
     * TODO: Bind once on init for corresponding graphics platform
     */

    // std::function<Camera *()> Camera::Create = CreateStub;
    // std::function<Camera *()> Camera::Create = CreateD3DCamera;
    std::function<Camera *()> Camera::Create = CreateOpenGLCamera;

} // namespace Iconoclast
