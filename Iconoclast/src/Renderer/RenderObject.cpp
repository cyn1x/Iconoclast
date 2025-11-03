#include "IconoclastPCH.h" // IWYU pragma: export

#include "D3DRenderObject.h"
#include "GraphicsContext.h"
#include "OpenGLRenderObject.h"
#include "RenderObject.h"

namespace Iconoclast {

    static RenderObject *CreateStub(GraphicsContext &)
    {
        // TODO: Handle logging for when function is not assigned
        return nullptr;
    }

    RenderObject *CreateD3DRenderObject(GraphicsContext &context)
    {
        return new D3DRenderObject(context);
    }

    RenderObject *CreateOpenGLRenderObject(GraphicsContext &context)
    {
        return new OpenGLRenderObject(context);
    }

    /*
     * TODO: Bind once on init for corresponding graphics platform
     */

    // std::function<RenderObject *(GraphicsContext &)> RenderObject::Create = CreateStub;
    // std::function<RenderObject *(GraphicsContext &)> RenderObject::Create =
    // CreateD3DRenderObject;
    std::function<RenderObject *(GraphicsContext &)> RenderObject::Create =
        CreateOpenGLRenderObject;

} // namespace Iconoclast
