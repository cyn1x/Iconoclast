#include "RenderObject.h"
#include <Iconoclast.h>

class Sandbox : public Iconoclast::Application
{
public:
    Sandbox()
    {
        // Test code
        m_Camera = Iconoclast::Camera::Create();
        m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

        m_RenderObject = Iconoclast::RenderObject::Create(*m_Context);

        // Set position of vertices and their respective colors
        Iconoclast::Vertex triangleVertices[] = {
            {-1.0f, -1.0f, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f}}, // Bottom left
            {0.0f,  1.0f,  0.0f, {1.0f, 0.0f, 0.0f, 1.0f}}, // Top middle
            {1.0f,  -1.0f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}}  // Bottom right
        };

        uint32_t vertexCount = sizeof(triangleVertices) / sizeof(Iconoclast::Vertex);

        uint32_t indicies[]  = {0, 1, 2, 0, 2, 3};
        uint32_t indexCount  = sizeof(indicies) / sizeof(uint32_t);

        m_RenderObject->CreateVertexBuffer(triangleVertices, vertexCount);
        m_RenderObject->CreateIndexBuffer(indicies, indexCount);
        // TODO: Abstract graphics API shader directories
        m_RenderObject->CreateShader("Assets/Shaders/DirectX/Color.vs.hlsl",
                                     "Assets/Shaders/DirectX/Color.ps.hlsl");

        Iconoclast::Vertex quadVertices[] = {
            {-1.0f, -1.0f, 0.0f}, // Bottom left
            {-1.0f, 1.0f,  0.0f}, // Top left
            {1.0f,  1.0f,  0.0f}, // Top Right
            {1.0f,  -1.0f, 0.0f}  // Bottom right
        };
    }

    ~Sandbox()
    {
        delete m_RenderObject;
        delete m_Camera;
    }

    void OnUpdate() override
    {
        // Clear the buffers to begin the scene.
        Iconoclast::Renderer::BeginScene(m_Camera);
        m_RenderObject->Render();
        Iconoclast::Renderer::EndScene();
    }

    void OnEvent(Iconoclast::Event &e) override
    {
        Iconoclast::Application::OnEvent(e);
    }

private:
    Iconoclast::Camera       *m_Camera;
    Iconoclast::RenderObject *m_RenderObject;
};

Iconoclast::Application *Iconoclast::CreateApplication()
{
    return new Sandbox;
}
