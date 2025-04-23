#include <Iconoclast.h>

class Sandbox : public Iconoclast::Application
{
public:
    Sandbox()
    {
        // Test code
        m_Camera = Iconoclast::Camera::Create();
        m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

        m_Mesh                                = Iconoclast::Mesh::Create(*m_Context);

        Iconoclast::Vertex triangleVertices[] = {
            {-1.0f, -1.0f, 0.0f}, // Bottom left
            {0.0f,  1.0f,  0.0f}, // Top middle
            {1.0f,  -1.0f, 0.0f}  // Bottom right
        };

        uint32_t vertexCount = sizeof(triangleVertices) / sizeof(Iconoclast::Vertex);

        uint32_t indicies[]  = {0, 1, 2, 0, 2, 3};
        uint32_t indexCount  = sizeof(indicies) / sizeof(uint32_t);

        m_Mesh->CreateVertexBuffer(triangleVertices, vertexCount);
        m_Mesh->CreateIndexBuffer(indicies, indexCount);
        m_Mesh->CreateShader("Assets/Shaders/VertexShader.hlsl", "Assets/Shaders/PixelShader.hlsl");
    }

    ~Sandbox()
    {
        delete m_Mesh;
        delete m_Camera;
    }

    void OnUpdate() override
    {
        // Clear the buffers to begin the scene.
        Iconoclast::Renderer::BeginScene(m_Camera);
        m_Mesh->Render();
        Iconoclast::Renderer::EndScene();
    }

    void OnEvent(Iconoclast::Event &e) override
    {
        Iconoclast::Application::OnEvent(e);
    }

private:
    Iconoclast::Camera *m_Camera;
    Iconoclast::Mesh   *m_Mesh;
};

Iconoclast::Application *Iconoclast::CreateApplication()
{
    return new Sandbox;
}
