#pragma once

#include "Camera.h"
#include "GraphicsContext.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4514)
#pragma warning(disable : 4820)

namespace Iconoclast {

    class RendererAPI
    {
    public:
        enum API
        {
            None    = 0,
            DirectX = 1
        };

    public:
        virtual ~RendererAPI()
        {
        }

        virtual void             Initialize(GraphicsContext *) = 0;
        virtual void             SetClearColor()               = 0;
        virtual void             Clear()                       = 0;
        virtual void             BeginScene(Camera *camera)    = 0;
        virtual void             EndScene()                    = 0;
        virtual void             Draw()                        = 0;
        virtual void             Shutdown()                    = 0;

        virtual GraphicsContext *GetContext()                  = 0;

        inline static API        GetAPI()
        {
            return s_API;
        }

    protected:
        bool m_VSync;
        bool m_FullScreen;

    private:
        static API s_API;
    };

} // namespace Iconoclast
#pragma warning(pop)
