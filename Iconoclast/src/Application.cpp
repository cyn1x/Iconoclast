#include "IconoclastPCH.h"

#define ICONOCLAST_EXPORTS

#include "Application.h"
#include "GraphicsContext.h"
#include "Renderer.h"

namespace Iconoclast {

    Application::Application()
    {
        // TODO: Abstract to system class
        auto       path = std::filesystem::current_path().string();
        auto const pos  = path.find_last_of('\\');
        const auto leaf = path.substr(pos + 1);

        if (leaf != "Crucible") {
            // TODO: Handle invalid working dir
        }

        m_Window = std::shared_ptr<Window>(Window::Create());
        m_Window->SetEventCallback([this](Event &e) { this->OnEvent(e); });

        m_Context = std::shared_ptr<GraphicsContext>(GraphicsContext::Create());
    }

    Application::~Application()
    {
        m_Context->ShutDown();
    }

    void Application::Run()
    {
        Renderer::Initialize(m_Context.get());

        while (m_Running) {
            this->OnUpdate();
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(
            [this](WindowCloseEvent &event) { return OnWindowClose(event); });
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

} // namespace Iconoclast
