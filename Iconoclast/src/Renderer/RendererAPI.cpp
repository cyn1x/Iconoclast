#include "IconoclastPCH.h" // IWYU pragma: export

#include "D3DContext.h"
#include "OpenGLContext.h"
#include "RendererAPI.h"

namespace Iconoclast {

    /*
     * TODO: Dynamic renderer platform enumerator selection
     */

    // RendererAPI::API RendererAPI::s_API = API::DirectX;
    RendererAPI::API RendererAPI::s_API = API::OpenGL;

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

} // namespace Iconoclast
