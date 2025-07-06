#include "IconoclastPCH.h" // IWYU pragma: keep

#include "D3DRendererAPI.h"
#include "OpenGLRendererAPI.h"
#include "Renderer.h"

namespace Iconoclast {

    /*
     * TODO: Dynamic renderer platform implementation selection
     */

    // RendererAPI *Renderer::s_RendererAPI = new D3DRendererAPI;
    RendererAPI *Renderer::s_RendererAPI = new OpenGLRendererAPI;
    Scene       *Renderer::s_Scene       = new Scene;

} // namespace Iconoclast
