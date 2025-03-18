#define ICONOCLAST_EXPORTS

#include "Application.h"
#include <memory>
#include <windows.h>

namespace Iconoclast {


    Application::Application()
    {
        m_Window = std::shared_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running) {
            m_Window->OnUpdate();
        }
    }


} // namespace Iconoclast
