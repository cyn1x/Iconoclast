#include "IconoclastPCH.h" // IWYU pragma: keep

#include "D3DRendererAPI.h"
#include "Renderer.h"

namespace Iconoclast {

    RendererAPI *Renderer::s_RendererAPI = new D3DRendererAPI;
    Scene       *Renderer::s_Scene       = new Scene;

} // namespace Iconoclast
