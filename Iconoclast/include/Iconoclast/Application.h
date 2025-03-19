#pragma once

#include "ApplicationEvent.h"
#include "Core.h"
#include "Event.h"
#include "Window.h"

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4820)

namespace Iconoclast {

    class Application
    {
    public:
        ICONOCLAST_API Application();
        ICONOCLAST_API virtual ~Application();

        ICONOCLAST_API void Run();

        void                OnEvent(Event &e);

    private:
        bool                    OnWindowClose(WindowCloseEvent &e);
        std::shared_ptr<Window> m_Window;
        bool                    m_Running = true;
    };

    Application *CreateApplication();

} // namespace Iconoclast

#pragma warning(pop)
