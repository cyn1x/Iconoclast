#include "IconoclastPCH.h" // IWYU pragma: keep

#include "OpenGLRendererAPI.h"

namespace Iconoclast {

    OpenGLRendererAPI::OpenGLRendererAPI()
    {
    }

    OpenGLRendererAPI::~OpenGLRendererAPI()
    {
    }

    void OpenGLRendererAPI::Initialize(GraphicsContext *context)
    {
        float fieldOfView, screenAspect;

        // Get the device context for this window.
        m_Context = dynamic_cast<OpenGLContext *>(context);

        // Set the depth buffer to be entirely cleared to 1.0 values.
        glClearDepth(1.0f);

        // Enable depth testing.
        glEnable(GL_DEPTH_TEST);

        // Set the polygon winding to front facing for the left handed system.
        glFrontFace(GL_CW);

        // Enable back face culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Initialize the world/model matrix to the identity matrix.
        BuildIdentityMatrix(m_WorldMatrix);

        // Set the field of view and screen aspect ratio.
        fieldOfView  = 3.14159265358979323846f / 4.0f;
        screenAspect = (float)m_Context->GetWidth() / (float)m_Context->GetHeight();

        // Build the perspective projection matrix.
        BuildPerspectiveFovLHMatrix(m_ProjectionMatrix, fieldOfView, screenAspect, m_ScreenNear,
                                    m_ScreenDepth);
    }

    void OpenGLRendererAPI::SetClearColor()
    {
    }

    void OpenGLRendererAPI::Clear()
    {
    }

    void OpenGLRendererAPI::BeginScene(Camera *camera)
    {
        // Set the color to clear the screen to.
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

        // Clear the screen and depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // SceneData *sceneData = Scene::GetSceneData();

        // Generate the view matrix based on the camera's position.
        // camera->Render(sceneData);

        // sceneData->worldMatrix      = m_WorldMatrix;
        // sceneData->projectionMatrix = m_ProjectionMatrix;
    }

    void OpenGLRendererAPI::EndScene()
    {
        m_Context->SwapBuffers();
    }

    void OpenGLRendererAPI::Draw()
    {
    }

    void OpenGLRendererAPI::Shutdown()
    {
    }

    void OpenGLRendererAPI::BuildIdentityMatrix(float *matrix)
    {
        matrix[0]  = 1.0f;
        matrix[1]  = 0.0f;
        matrix[2]  = 0.0f;
        matrix[3]  = 0.0f;

        matrix[4]  = 0.0f;
        matrix[5]  = 1.0f;
        matrix[6]  = 0.0f;
        matrix[7]  = 0.0f;

        matrix[8]  = 0.0f;
        matrix[9]  = 0.0f;
        matrix[10] = 1.0f;
        matrix[11] = 0.0f;

        matrix[12] = 0.0f;
        matrix[13] = 0.0f;
        matrix[14] = 0.0f;
        matrix[15] = 1.0f;
    }

    void OpenGLRendererAPI::BuildPerspectiveFovLHMatrix(float *matrix, float fieldOfView,
                                                        float screenAspect, float screenNear,
                                                        float screenDepth)
    {
        matrix[0]  = 1.0f / (screenAspect * tan(fieldOfView * 0.5f));
        matrix[1]  = 0.0f;
        matrix[2]  = 0.0f;
        matrix[3]  = 0.0f;

        matrix[4]  = 0.0f;
        matrix[5]  = 1.0f / tan(fieldOfView * 0.5f);
        matrix[6]  = 0.0f;
        matrix[7]  = 0.0f;

        matrix[8]  = 0.0f;
        matrix[9]  = 0.0f;
        matrix[10] = screenDepth / (screenDepth - screenNear);
        matrix[11] = 1.0f;

        matrix[12] = 0.0f;
        matrix[13] = 0.0f;
        matrix[14] = (-screenNear * screenDepth) / (screenDepth - screenNear);
        matrix[15] = 0.0f;
    }

    void OpenGLRendererAPI::MatrixRotationY(float *matrix, float angle)
    {
        matrix[0]  = cosf(angle);
        matrix[1]  = 0.0f;
        matrix[2]  = -sinf(angle);
        matrix[3]  = 0.0f;

        matrix[4]  = 0.0f;
        matrix[5]  = 1.0f;
        matrix[6]  = 0.0f;
        matrix[7]  = 0.0f;

        matrix[8]  = sinf(angle);
        matrix[9]  = 0.0f;
        matrix[10] = cosf(angle);
        matrix[11] = 0.0f;

        matrix[12] = 0.0f;
        matrix[13] = 0.0f;
        matrix[14] = 0.0f;
        matrix[15] = 1.0f;
    }

    void OpenGLRendererAPI::MatrixTranslation(float *matrix, float x, float y, float z)
    {
        matrix[0]  = 1.0f;
        matrix[1]  = 0.0f;
        matrix[2]  = 0.0f;
        matrix[3]  = 0.0f;

        matrix[4]  = 0.0f;
        matrix[5]  = 1.0f;
        matrix[6]  = 0.0f;
        matrix[7]  = 0.0f;

        matrix[8]  = 0.0f;
        matrix[9]  = 0.0f;
        matrix[10] = 1.0f;
        matrix[11] = 0.0f;

        matrix[12] = x;
        matrix[13] = y;
        matrix[14] = z;
        matrix[15] = 1.0f;
    }

    void OpenGLRendererAPI::MatrixMultiply(float *result, float *matrix1, float *matrix2)
    {
        result[0] = (matrix1[0] * matrix2[0]) + (matrix1[1] * matrix2[4]) +
                    (matrix1[2] * matrix2[8]) + (matrix1[3] * matrix2[12]);
        result[1] = (matrix1[0] * matrix2[1]) + (matrix1[1] * matrix2[5]) +
                    (matrix1[2] * matrix2[9]) + (matrix1[3] * matrix2[13]);
        result[2] = (matrix1[0] * matrix2[2]) + (matrix1[1] * matrix2[6]) +
                    (matrix1[2] * matrix2[10]) + (matrix1[3] * matrix2[14]);
        result[3] = (matrix1[0] * matrix2[3]) + (matrix1[1] * matrix2[7]) +
                    (matrix1[2] * matrix2[11]) + (matrix1[3] * matrix2[15]);

        result[4] = (matrix1[4] * matrix2[0]) + (matrix1[5] * matrix2[4]) +
                    (matrix1[6] * matrix2[8]) + (matrix1[7] * matrix2[12]);
        result[5] = (matrix1[4] * matrix2[1]) + (matrix1[5] * matrix2[5]) +
                    (matrix1[6] * matrix2[9]) + (matrix1[7] * matrix2[13]);
        result[6] = (matrix1[4] * matrix2[2]) + (matrix1[5] * matrix2[6]) +
                    (matrix1[6] * matrix2[10]) + (matrix1[7] * matrix2[14]);
        result[7] = (matrix1[4] * matrix2[3]) + (matrix1[5] * matrix2[7]) +
                    (matrix1[6] * matrix2[11]) + (matrix1[7] * matrix2[15]);

        result[8] = (matrix1[8] * matrix2[0]) + (matrix1[9] * matrix2[4]) +
                    (matrix1[10] * matrix2[8]) + (matrix1[11] * matrix2[12]);
        result[9] = (matrix1[8] * matrix2[1]) + (matrix1[9] * matrix2[5]) +
                    (matrix1[10] * matrix2[9]) + (matrix1[11] * matrix2[13]);
        result[10] = (matrix1[8] * matrix2[2]) + (matrix1[9] * matrix2[6]) +
                     (matrix1[10] * matrix2[10]) + (matrix1[11] * matrix2[14]);
        result[11] = (matrix1[8] * matrix2[3]) + (matrix1[9] * matrix2[7]) +
                     (matrix1[10] * matrix2[11]) + (matrix1[11] * matrix2[15]);

        result[12] = (matrix1[12] * matrix2[0]) + (matrix1[13] * matrix2[4]) +
                     (matrix1[14] * matrix2[8]) + (matrix1[15] * matrix2[12]);
        result[13] = (matrix1[12] * matrix2[1]) + (matrix1[13] * matrix2[5]) +
                     (matrix1[14] * matrix2[9]) + (matrix1[15] * matrix2[13]);
        result[14] = (matrix1[12] * matrix2[2]) + (matrix1[13] * matrix2[6]) +
                     (matrix1[14] * matrix2[10]) + (matrix1[15] * matrix2[14]);
        result[15] = (matrix1[12] * matrix2[3]) + (matrix1[13] * matrix2[7]) +
                     (matrix1[14] * matrix2[11]) + (matrix1[15] * matrix2[15]);
    }

    GraphicsContext *OpenGLRendererAPI::GetContext()
    {
        return m_Context;
    }

} // namespace Iconoclast
