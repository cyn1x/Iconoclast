#include "IconoclastPCH.h" // IWYU pragma: export

#include "D3DContext.h"
#include "RendererAPI.h"

namespace Iconoclast {

    RendererAPI::API RendererAPI::s_API = API::DirectX;

    GraphicsContext *GraphicsContext::Create(const ContextProps &props)
    {
        RendererAPI::API API = RendererAPI::GetAPI();

        switch (API) {
            case RendererAPI::API::DirectX:
                return new D3DContext(props);
            case RendererAPI::API::None:
                break;
        }

        return nullptr;
    }

} // namespace Iconoclast
