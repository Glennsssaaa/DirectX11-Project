#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    this->windowWidth = width;
    this->windowHeight = height;
    this->fpsTimer.Start();
    if (!InitializeDirectX(hwnd)) {
        return false;
    }

    if (!InitializeShaders()) {
        return false;
    }

    if (!IntiializeScene()) {
        return false;
    }

    //ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
    ImGui::StyleColorsDark();

    return true;
}

void Graphics::RenderFrame()
{
    //Declare States
    float bgcolor[] = { 0.0f,0.0f,0.0f,1.0f };
    this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
    this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    this->deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
    this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->deviceContext->RSSetState(this->rasterizerState.Get());
    this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
    this->deviceContext->OMSetBlendState(this->blendState.Get(), NULL, 0xFFFFFFFF);
    this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
    this->deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
    this->deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

    UINT offset = 0;
    static float translationOffset[3] = { 0,0,0 };
    static float scaleOffset[3] = { 5,5,5 };
    static float rotationOffset[3] = { 0,0,0 };

    //Opaque Objects
    /*
    { // Brick
       //Create World Matrix
        static float translationOffset[3] = { 0,0,4.0f };
        static float scaleOffset[3] = { 5.0f,5.0f,5.0f };
        static float rotationOffset[3] = { 0,0,0 };

        DirectX::XMMATRIX world = XMMatrixScaling(scaleOffset[0], scaleOffset[1], scaleOffset[2]) * XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);


        //Update vertex shader constant buffer
        cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);
        if (!cb_vs_vertexshader.ApplyChanges()) {
            return;
        }
        this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

        //Update pixel shader constant buffer
        cb_ps_pixelshader.data.alpha = 1.0f;
        cb_ps_pixelshader.ApplyChanges();
        this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        //Draw Shape
        this->deviceContext->PSSetShaderResources(0, 1, this->brickTexture.GetAddressOf());
        this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
        this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
    }
    { // Grass
        //Create World Matrix
        DirectX::XMMATRIX world = XMMatrixScaling(scaleOffset[0], scaleOffset[1], scaleOffset[2]) * XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);

        //Update vertex shader constant buffer
        cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);
        if (!cb_vs_vertexshader.ApplyChanges()) {
            return;
        }
        this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

        //Update pixel shader constant buffer
        cb_ps_pixelshader.data.alpha = 1.0f;
        cb_ps_pixelshader.ApplyChanges();
        this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        //Draw Shape
        this->deviceContext->PSSetShaderResources(0, 1, this->grassTexture.GetAddressOf());
        this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
        this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
    }*/


    ///Transparent Objects
    static float alphaValue = 1.0f;
    { // Kong
       //Create World Matrix
        static float translationOffset[3] = { 0,0,-1.0f };
        static float scaleOffset[3] = { 1,1,1 };
        static float rotationOffset[3] = { 0,0,0 };

        DirectX::XMMATRIX world = XMMatrixScaling(scaleOffset[0], scaleOffset[1], scaleOffset[2]) * XMMatrixTranslation(translationOffset[0], translationOffset[1], translationOffset[2]);


        //Update vertex shader constant buffer
        cb_vs_vertexshader.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();
        cb_vs_vertexshader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexshader.data.mat);
        if (!cb_vs_vertexshader.ApplyChanges()) {
            return;
        }
        this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexshader.GetAddressOf());

        //Update pixel shader constant buffer
        cb_ps_pixelshader.data.alpha = alphaValue;
        cb_ps_pixelshader.ApplyChanges();
        this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_pixelshader.GetAddressOf());

        //Draw Shape
        this->deviceContext->PSSetShaderResources(0, 1, this->brickTexture.GetAddressOf());
        this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
        this->deviceContext->IASetIndexBuffer(indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        this->deviceContext->RSSetState(this->rasterizerState_CullFront.Get());
        this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
        this->deviceContext->RSSetState(this->rasterizerState.Get());
        this->deviceContext->DrawIndexed(indicesBuffer.BufferSize(), 0, 0);
    }
    //Draw Text
    static int fpsCounter = 0;
    static std::string fpsString = "FPS: 0";
    fpsCounter += 1;
    if (fpsTimer.GetMillisecondsElapsed() > 1000.0) {
        fpsString = "FPS: " + std::to_string(fpsCounter);
        fpsCounter = 0;
        fpsTimer.Restart();
    }

    spriteBatch->Begin();
    spriteFont->DrawString(spriteBatch.get(), StringConverter::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
    spriteBatch->End();

    static bool enableWireframe = false;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");
    ImGui::DragFloat3("Translation X/Y/Z", translationOffset, 0.1f, -5.0f, 5.0f);
    ImGui::DragFloat3("Scale X/Y/Z", scaleOffset, 0.1f, 0.0f, 50.0f);
    ImGui::DragFloat3("Rotation X/Y/Z", rotationOffset , 0.1f, 0.0f, 6.0f);
    ImGui::DragFloat("Alpha", &alphaValue, 0.01f, 0.0f, 1.0f);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    //Enable VSYNC - 1
    this->swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
    //Check Adapters and setup Swap & Chain
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    if (adapters.size() < 1) {
        ErrorLogger::Log("No DXGI Adapters found");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferDesc.Width = this->windowWidth;
    scd.BufferDesc.Height = this->windowHeight;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = true;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr;
    hr = D3D11CreateDeviceAndSwapChain(
        adapters[0].pAdapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,
        NULL,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &scd,
        this->swapchain.GetAddressOf(),
        this->device.GetAddressOf(),
        NULL,
        this->deviceContext.GetAddressOf());

    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create device and swapchain");
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "GetBuffer Failed");
        return false;
    }

    hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create render target view");
        return false;
    }

    //Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = this->windowWidth;
    depthStencilDesc.Height = this->windowHeight;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    
    hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create depth stencil buffer");
        return false;
    }
    hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create depth stencil view");
        return false;
    }

    this->deviceContext->OMSetRenderTargets(1,this->renderTargetView.GetAddressOf(),this->depthStencilView.Get());

    //Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthstencildesc;
    ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthstencildesc.DepthEnable = true;
    depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
    if (hr != S_OK) {
        ErrorLogger::Log(hr, "Failed to create depth stencil state");
        return false;
    }


    //Viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = this->windowWidth;
    viewport.Height = this->windowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    //Set Viewport
    this->deviceContext->RSSetViewports(1, &viewport);

    //Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
    if (hr != S_OK) {
        ErrorLogger::Log(hr, "Failed to create rasterizer state");
        return false;
    }

    //Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc_CullFront;
    ZeroMemory(&rasterizerDesc_CullFront, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc_CullFront.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc_CullFront.CullMode = D3D11_CULL_FRONT;
    hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
    if (hr != S_OK) {
        ErrorLogger::Log(hr, "Failed to create rasterizer state");
        return false;
    }

    //Blend State
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(rtbd));

    rtbd.BlendEnable = true;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.RenderTarget[0] = rtbd;

    hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
    if (hr != S_OK) {
        ErrorLogger::Log(hr, "Failed to create blend sate");
        return false;
    }

    //Font Batch
    spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
    spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms.spritefont");

    //Sampler State
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = this->device->CreateSamplerState(&samplerDesc, this->samplerState.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create sampler state");
        return false;
    }

    return true;
}

bool Graphics::InitializeShaders()
{
    std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
    if (IsDebuggerPresent() == TRUE) {
#ifdef _DEBUG
#ifdef _WIN64
        shaderfolder = L"..\\x64\\Debug\\";
#else 
        shaderfolder = L"..\\Debug\\";
    }
#endif
#else
#ifdef _WIN64
        shaderfolder = L"..\\x64\\Release\\";
#else 
        shaderfolder = L"..\\Release\\"
#endif
#endif
    }



    D3D11_INPUT_ELEMENT_DESC layout[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA ,0}

    };

    UINT numElements = ARRAYSIZE(layout);

    if (!vertexshader.Initialize(this->device, shaderfolder + L"vertexshader.cso", layout, numElements)) {
        return false;
    }

    if (!pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso")) {
        return false;
    }

    return true;
}

bool Graphics::IntiializeScene()
{
    //Create Shape
    Vertex v[] = {
        Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f), //FRONT Bottom Left   - [0]
        Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f), //FRONT Top Left      - [1]
        Vertex(0.5f,   0.5f, -0.5f, 1.0f, 0.0f), //FRONT Top Right     - [2]
        Vertex(0.5f,  -0.5f, -0.5f, 1.0f, 1.0f), //FRONT Bottom Right   - [3]
        Vertex(-0.5f,  -0.5f, 0.5f, 0.0f, 1.0f), //BACK Bottom Left   - [4]
        Vertex(-0.5f,   0.5f, 0.5f, 0.0f, 0.0f), //BACK Top Left      - [5]
        Vertex(0.5f,   0.5f, 0.5f, 1.0f, 0.0f), //BACK Top Right     - [6]
        Vertex(0.5f,  -0.5f, 0.5f, 1.0f, 1.0f), //BACK Bottom Right   - [7]
    };

   
    //Load Vertex Data
    HRESULT hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create vertex buffer");
        return false;
    }


    DWORD indices[] = {
        0, 1, 2, //FRONT
        0, 2, 3, //FRONT
        4, 7, 6, //BACK 
        4, 6, 5, //BACK
        3, 2, 6, //RIGHT SIDE
        3, 6, 7, //RIGHT SIDE
        4, 5, 1, //LEFT SIDE
        4, 1, 0, //LEFT SIDE
        1, 5, 6, //TOP
        1, 6, 2, //TOP
        0, 3, 7, //BOTTOM
        0, 7, 4, //BOTTOM
    };

    //Load Index Data
    hr = this->indicesBuffer.Initialize(this->device.Get(), indices, ARRAYSIZE(indices));
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create indices buffer");
        return hr;
    }

    //Load Texture
    hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\DK.png", nullptr, kongTexture.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create wic texture from file");
        return false;
    }

    hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\grass.jpg", nullptr, grassTexture.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create wic texture from file");
        return false;
    }

    hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\brick.jpg", nullptr, brickTexture.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to create wic texture from file");
        return false;
    }

    //Initialize vertex shader constant buffer
    hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to initialize constant buffer");
        return false;
    }
    //Initialize pixel shader constant buffer
    hr = this->cb_ps_pixelshader.Initialize(this->device.Get(), this->deviceContext.Get());
    if (FAILED(hr)) {
        ErrorLogger::Log(hr, "Failed to initialize constant buffer");
        return false;
    }

    camera.SetPosition(0.0f, 0.0f, -2.0f);
    camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.f);

    return true;
}