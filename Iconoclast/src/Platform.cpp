#include "IconoclastPCH.h"

#include "GraphicsContext.h"
#include "Platform.h"
#include "Renderer.h"
#include "RendererAPI.h"

#include "D3DRendererAPI.h"
#include "OpenGLRendererAPI.h"

#include "D3DContext.h"
#include "OpenGLContext.h"

#include "D3DCamera.h"
#include "OpenGLCamera.h"

#include "D3DRenderObject.h"
#include "OpenGLRenderObject.h"

namespace Iconoclast {

    /*
     * Scene
     */

    Scene *Renderer::s_Scene = new Scene;

    /*
     *  TODO: Move to a configuration based platform loader
     */

    /*
     * Renderer
     */

    RendererAPI *Renderer::s_RendererAPI = new D3DRendererAPI;
    // RendererAPI *Renderer::s_RendererAPI = new OpenGLRendererAPI;
    // RendererAPI *Renderer::s_RendererAPI = nullptr;

    /*
     * RendererAPI
     */

    RendererAPI::API RendererAPI::s_API = API::DirectX;
    // RendererAPI::API RendererAPI::s_API = API::OpenGL;
    // RendererAPI::API RendererAPI::s_API = API::None;

    /*
     * GraphicsContext
     */

    GraphicsContext *GraphicsContext::Create(const ContextProps &props)
    {
        RendererAPI::API API = RendererAPI::GetAPI();

        switch (API) {
            case RendererAPI::API::DirectX:
                return new D3DContext(props);
            case RendererAPI::API::OpenGL:
                return new OpenGLContext(props);
            case RendererAPI::API::None:
                break;
        }

        return nullptr;
    }

    /*
     * Camera
     */

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

    std::function<Camera *()> Camera::Create = CreateD3DCamera;
    // std::function<Camera *()> Camera::Create = CreateOpenGLCamera;
    // std::function<Camera *()> Camera::Create = CreateStub;

    /*
     * RenderObject
     */

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

    std::function<RenderObject *(GraphicsContext &)> RenderObject::Create = CreateD3DRenderObject;
    // std::function<RenderObject *(GraphicsContext &)> RenderObject::Create =
    //     CreateOpenGLRenderObject;
    // std::function<RenderObject *(GraphicsContext &)> RenderObject::Create = CreateStub;

    Platform::Platform()
    {
        Initialize();
    }

    Platform::~Platform()
    {
    }

    void Platform::Initialize()
    {
        // TODO: Move static platform initialization here
    }

} // namespace Iconoclast
