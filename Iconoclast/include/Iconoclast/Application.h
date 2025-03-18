#pragma once

#include "Core.h"
#include "Window.h"
#include <memory>

static_assert(true, "");
#pragma warning(push)
#pragma warning(disable : 4820)

namespace Iconoclast {

    class Application
    {
    public:
        ICONOCLAST_API Application();
        ICONOCLAST_API virtual ~Application();

        ICONOCLAST_API virtual void Run();


    private:
        std::shared_ptr<Window> m_Window;
        bool                    m_Running = true;
    };

    Application *CreateApplication();

} // namespace Iconoclast

#pragma warning(pop)
