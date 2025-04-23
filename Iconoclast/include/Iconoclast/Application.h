#pragma once

#include "ApplicationEvent.h"
#include "Event.h"
#include "GraphicsContext.h"
#include "Window.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4820)

namespace Iconoclast {

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void         Run();
        virtual void OnEvent(Event &e);

        virtual void OnUpdate() = 0;

    protected:
        std::shared_ptr<GraphicsContext> m_Context;

    private:
        std::shared_ptr<Window> m_Window;
        bool                    m_Running = true;
        bool                    OnWindowClose(WindowCloseEvent &e);
    };

    Application *CreateApplication();

} // namespace Iconoclast

#pragma warning(pop)
