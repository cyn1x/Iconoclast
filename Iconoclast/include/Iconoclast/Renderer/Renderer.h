#pragma once

#include "RendererAPI.h"
#include "Scene.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)

namespace Iconoclast {

    class Renderer
    {
    public:
        inline static void Initialize(GraphicsContext *context)
        {
            s_RendererAPI->Initialize(context);
        }

        inline static void SetClearColor()
        {
            s_RendererAPI->SetClearColor();
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }

        inline static void BeginScene(Camera *camera)
        {
            s_RendererAPI->BeginScene(camera);
        }

        inline static void EndScene()
        {
            s_RendererAPI->EndScene();
        }

        inline static void Draw()
        {
            s_RendererAPI->Draw();
        }

    private:
        static RendererAPI *s_RendererAPI;
        static Scene       *s_Scene;
    };

} // namespace Iconoclast
#pragma warning(pop)
