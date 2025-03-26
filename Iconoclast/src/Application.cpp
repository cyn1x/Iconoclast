#include "IconoclastPCH.h"

#define ICONOCLAST_EXPORTS

#include "Application.h"
#include "D3DContext.h"
#include "GraphicsContext.h"

namespace Iconoclast {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        m_Window = std::shared_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_Context = std::shared_ptr<GraphicsContext>(GraphicsContext::Create());
    }

    Application::~Application()
    {
        m_Context->ShutDown();
    }

    void Application::Run()
    {
        while (m_Running) {
            m_Context->BeginScene();
            m_Context->EndScene();
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

} // namespace Iconoclast
