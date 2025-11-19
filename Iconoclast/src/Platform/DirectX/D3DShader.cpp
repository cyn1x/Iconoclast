#include "IconoclastPCH.h"

#include "D3DContext.h"
#include "D3DShader.h"

namespace Iconoclast {

    D3DShader::D3DShader(GraphicsContext &context, const std::filesystem::path &vertexSrc,
                         const std::filesystem::path &pixelSrc)
    {
        m_VertexSrc    = vertexSrc;
        m_PixelSrc     = pixelSrc;

        m_VertexShader = 0;
        m_PixelShader  = 0;
        m_Layout       = 0;
        m_MatrixBuffer = 0;

        bool    result;
        wchar_t vsFilename[128];
        wchar_t psFilename[128];
        int     error;

        // Set the filename of the vertex shader.
        auto wideVertexSrc = m_VertexSrc.wstring();
        error              = wcscpy_s(vsFilename, 128, wideVertexSrc.c_str());
        if (error != 0) {
            // TODO: Handle error
        }

        // Set the filename of the pixel shader.
        auto widePixelSrc = m_PixelSrc.wstring();
        error             = wcscpy_s(psFilename, 128, widePixelSrc.c_str());
        if (error != 0) {
            // TODO: Handle error
        }

        D3DContext *ctx = static_cast<D3DContext *>(&context);

        // Initialize the vertex and pixel shaders.
        result = Initialize(ctx->GetDevice(), ctx->GetWindowHandle(), vsFilename, psFilename);
        if (!result) {
            // TODO: Handle error
        }
    }

    D3DShader::~D3DShader()
    {
    }

    HRESULT D3DShader::Initialize(ID3D11Device *device, HWND hwnd, WCHAR *vsFilename,
                                  WCHAR *psFilename)
    {
        HRESULT                  result;
        ID3D10Blob              *errorMessage;
        ID3D10Blob              *vertexShaderBuffer;
        ID3D10Blob              *pixelShaderBuffer;
        D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
        unsigned int             numElements;
        D3D11_BUFFER_DESC        matrixBufferDesc;

        // Initialize the pointers this function will use to null.
        errorMessage       = 0;
        vertexShaderBuffer = 0;
        pixelShaderBuffer  = 0;

        // Compile the vertex shader code.
        result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0",
                                    D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer,
                                    &errorMessage);
        if (FAILED(result)) {
            // If the shader failed to compile it should have writen something to the error message.
            if (errorMessage) {
                OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
            }
            // If there was  nothing in the error message then it simply could not find the shader
            // file itself.
            else {
                MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
            }

            return result;
        }

        // Compile the pixel shader code.
        result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0",
                                    D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer,
                                    &errorMessage);
        if (FAILED(result)) {
            // If the shader failed to compile it should have writen something to the error message.
            if (errorMessage) {
                OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
            }
            // If there was nothing in the error message then it simply could not find the file
            // itself.
            else {
                MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
            }

            return result;
        }

        // Create the vertex shader from the buffer.
        result =
            device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
                                       vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
        if (FAILED(result)) {
            return result;
        }

        // Create the pixel shader from the buffer.
        result =
            device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
                                      pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
        if (FAILED(result)) {
            return result;
        }

        // Create the vertex input layout description.
        // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
        polygonLayout[0].SemanticName         = "POSITION";
        polygonLayout[0].SemanticIndex        = 0;
        polygonLayout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
        polygonLayout[0].InputSlot            = 0;
        polygonLayout[0].AlignedByteOffset    = 0;
        polygonLayout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[0].InstanceDataStepRate = 0;

        polygonLayout[1].SemanticName         = "COLOR";
        polygonLayout[1].SemanticIndex        = 0;
        polygonLayout[1].Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
        polygonLayout[1].InputSlot            = 0;
        polygonLayout[1].AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
        polygonLayout[1].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[1].InstanceDataStepRate = 0;

        // Get a count of the elements in the layout.
        numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

        // Create the vertex input layout.
        result = device->CreateInputLayout(polygonLayout, numElements,
                                           vertexShaderBuffer->GetBufferPointer(),
                                           vertexShaderBuffer->GetBufferSize(), &m_Layout);
        if (FAILED(result)) {
            return result;
        }

        // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
        vertexShaderBuffer->Release();
        vertexShaderBuffer = 0;

        pixelShaderBuffer->Release();
        pixelShaderBuffer = 0;

        // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
        matrixBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth           = sizeof(MatrixBufferType);
        matrixBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags           = 0;
        matrixBufferDesc.StructureByteStride = 0;

        // Create the constant buffer pointer so we can access the vertex shader constant buffer
        // from within this class.
        result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
        if (FAILED(result)) {
            return result;
        }

        return result;
    }

    void D3DShader::Bind(GraphicsContext &context, const SceneData &sceneData) const
    {
        HRESULT                  result;
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        MatrixBufferType        *dataPtr;
        unsigned int             bufferNumber;

        D3DContext              *ctx = static_cast<D3DContext *>(&context);

        // Transpose the matrices to prepare them for the shader.
        // XMMATRIX worldMatrix      = XMMatrixTranspose(sceneData.worldMatrix);
        // XMMATRIX viewMatrix       = XMMatrixTranspose(sceneData.viewMatrix);
        // XMMATRIX projectionMatrix = XMMatrixTranspose(sceneData.projectionMatrix);

        // Lock the constant buffer so it can be written to.
        result = ctx->GetDeviceContext()->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
                                              &mappedResource);
        if (FAILED(result)) {
            // TODO: Handle error
        }

        // Get a pointer to the data in the constant buffer.
        dataPtr = (MatrixBufferType *)mappedResource.pData;

        // Copy the matrices into the constant buffer.
        // dataPtr->world      = worldMatrix;
        // dataPtr->view       = viewMatrix;
        // dataPtr->projection = projectionMatrix;

        // Unlock the constant buffer.
        ctx->GetDeviceContext()->Unmap(m_MatrixBuffer, 0);

        // Set the position of the constant buffer in the vertex shader.
        bufferNumber = 0;

        // Finanly set the constant buffer in the vertex shader with the updated values.
        ctx->GetDeviceContext()->VSSetConstantBuffers(bufferNumber, 1, &m_MatrixBuffer);

        // Set the vertex input layout.
        ctx->GetDeviceContext()->IASetInputLayout(m_Layout);

        // Set the vertex and pixel shaders that will be used to render this triangle.
        ctx->GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
        ctx->GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
    }

    void D3DShader::Unbind() const
    {
    }

    void D3DShader::Shutdown()
    {
        // Release the matrix constant buffer.
        if (m_MatrixBuffer) {
            m_MatrixBuffer->Release();
            m_MatrixBuffer = 0;
        }

        // Release the layout.
        if (m_Layout) {
            m_Layout->Release();
            m_Layout = 0;
        }

        // Release the pixel shader.
        if (m_PixelShader) {
            m_PixelShader->Release();
            m_PixelShader = 0;
        }

        // Release the vertex shader.
        if (m_VertexShader) {
            m_VertexShader->Release();
            m_VertexShader = 0;
        }
    }

    void D3DShader::OutputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd,
                                             WCHAR *shaderFilename)
    {
        char              *compileErrors;
        unsigned long long bufferSize, i;
        std::ofstream      fout;

        // Get a pointer to the error message text buffer.
        compileErrors = (char *)(errorMessage->GetBufferPointer());

        // Get the length of the message.
        bufferSize = errorMessage->GetBufferSize();

        // Open a file to write the error message to.
        fout.open("shader-error.txt");

        // Write out the error message.
        for (i = 0; i < bufferSize; i++) {
            fout << compileErrors[i];
        }

        // Close the file.
        fout.close();

        // Release the error message.
        errorMessage->Release();
        errorMessage = 0;

        // Pop a message up on the screen to notify the user to check the text file for compile
        // errors.
        MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.",
                   shaderFilename, MB_OK);
    }

} // namespace Iconoclast
