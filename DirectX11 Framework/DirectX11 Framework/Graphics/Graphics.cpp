#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    fpsTimer.Start();
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
    ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
    ImGui::StyleColorsDark();

    return true;
}

void Graphics::RenderFrame()
{
    static bool enableWireframe = false;
    cb_ps_light.data.dynamicLightColour = light.lightColour;
    cb_ps_light.data.dynamicLightStrength = light.lightStrength;
    cb_ps_light.data.dynamicLightPosition = light.GetPositionFloat3();
	cb_ps_light.data.dynamicLightAttenuation_a = light.attenuation_a;
	cb_ps_light.data.dynamicLightAttenuation_b = light.attenuation_b;
	cb_ps_light.data.dynamicLightAttenuation_c = light.attenuation_c;

    cb_ps_light.ApplyChanges();
    deviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddressOf());
    //Declare States
    float bgcolor[] = { 0.0f,0.0f,0.0f,1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
    deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    if (enableWireframe) {
        deviceContext->RSSetState(rasterizerState_Wireframe.Get());
    }
    else if (!enableWireframe) {
        deviceContext->RSSetState(rasterizerState.Get());
    }
    deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
    deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());


    //Stencil Mask
    /*deviceContext->OMSetDepthStencilState(depthStencilState_drawMask.Get(), 0);
    deviceContext->IASetInputLayout(vertexshader_2d.GetInputLayout());
    deviceContext->PSSetShader(pixelshader_2d.GetShader(), NULL, 0);
    deviceContext->VSSetShader(vertexshader_2d.GetShader(), NULL, 0);
    sprite.Draw(camera2D.GetWorldMatrix() * camera2D.GetOrthoMatrix());
    //Stencil Mask
    deviceContext->OMSetDepthStencilState(depthStencilState_applyMask.Get(), 0);
    */
    
    
    UINT offset = 0;
    static float translationOffset[3] = { 0,0,0 };
    static float scaleOffset[3] = { 5,5,5 };
    static float rotationOffset[3] = { 0,0,0 };

    static float alphaValue = 1.0f;

    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
    deviceContext->IASetInputLayout(vertexshader.GetInputLayout());
    deviceContext->VSSetShader(vertexshader.GetShader(), NULL, 0);
    deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);

    
    { // Car
        carModel.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
        carModel.SetPosition(15.0f,0,0);
        carModel.SetScale(0.01,0.01,0.01);
        //carModel.SetRotation(rotationOffset[0], rotationOffset[1], rotationOffset[2]);

    }
    { // Nanosuit
        nanosuitModel.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
        nanosuitModel.SetPosition(-15.0f,0,0);
        nanosuitModel.SetScale(1,1,1);
        // nanosuitModel.SetRotation(rotationOffset[0], rotationOffset[1], rotationOffset[2]);

    }
    { // Cow
        this->deviceContext->PSSetShaderResources(0, 1, this->cowTexture.GetAddressOf());
        cowModel.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
        cowModel.SetPosition(0,0,0);
        cowModel.SetScale(0.05,0.05,0.05);
    }
    {
        deviceContext->PSSetShader(pixelshader_nolight.GetShader(), NULL, 0);

        light.SetScale(5, 5, 5);

        if (enableFlashlight) {
            XMVECTOR lightPosition = Camera3D.GetPositionVector();
            lightPosition += Camera3D.GetForwardVector();
            light.SetPosition(lightPosition);
            light.SetRotation(Camera3D.GetRotationVector());
        }
        else {
			light.SetPosition(light.GetPositionVector());
			light.SetRotation(light.GetRotationVector());
            light.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
        }
    }
    { // Cube
        this->deviceContext->PSSetShaderResources(0, 1, this->brickTexture.GetAddressOf());
        cube.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
        cube.SetPosition(translationOffset[0], translationOffset[1], translationOffset[2]);
        cube.SetScale(scaleOffset[0], scaleOffset[1], scaleOffset[2]);
    }

    { // Sphere
        deviceContext->RSSetState(rasterizerState_CullFront.Get());
        this->deviceContext->PSSetShaderResources(0, 1, this->skyboxTexture.GetAddressOf());
        skyBox.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
        skyBox.SetPosition(Camera3D.GetPositionFloat3());
        skyBox.SetScale(25,25,25);
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
    spriteFont->DrawString(spriteBatch.get(), StringHelper::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
    spriteBatch->End();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Object Controls");
    ImGui::DragFloat3("Translation X/Y/Z", translationOffset, 0.1f, -5.0f, 5.0f);
    ImGui::DragFloat3("Scale X/Y/Z", scaleOffset, 0.1f, 0.0f, 50.0f);
    ImGui::DragFloat3("Rotation X/Y/Z", rotationOffset , 0.1f, 0.0f, 6.0f);
    ImGui::DragFloat("Alpha", &alphaValue, 0.01f, 0.0f, 1.0f);
    ImGui::Checkbox("Enable Wireframe", &enableWireframe);
    ImGui::End();

	ImGui::Begin("Light Controls");
	ImGui::DragFloat3("Ambient Light Colour", &cb_ps_light.data.ambientLightColour.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Light Strength", &cb_ps_light.data.ambientLightStrength, 0.01f, 0.0f, 1.0f);

	ImGui::DragFloat3("Diffuse Light Colour", &light.lightColour.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Diffuse Light Strength", &light.lightStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Light Attenuation A", &light.attenuation_a, 0.1f, 0.1f, 100.0f);
    ImGui::DragFloat("Light Attenuation B", &light.attenuation_b, 0.1f, 0.1f, 100.0f);
    ImGui::DragFloat("Light Attenuation C", &light.attenuation_c, 0.1f, 0.1f, 100.0f);

	ImGui::Checkbox("Enable Flashlight", &enableFlashlight);
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    //Enable VSYNC - 1
    swapchain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
    try{
    //Check Adapters and setup Swap & Chain
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    if (adapters.size() < 1) {
        ErrorLogger::Log("No DXGI Adapters found");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC scd = { 0 };

    scd.BufferDesc.Width = windowWidth;
    scd.BufferDesc.Height = windowHeight;
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

    COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain");

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    COM_ERROR_IF_FAILED(hr, "GetBuffer Failed");

    hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create render target view");

    //Depth/Stencil Buffer
    CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, windowWidth,windowHeight);
    depthStencilTextureDesc.MipLevels = 1;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    
    hr = device->CreateTexture2D(&depthStencilTextureDesc, NULL, depthStencilBuffer.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer");

    hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view");

    deviceContext->OMSetRenderTargets(1,renderTargetView.GetAddressOf(),depthStencilView.Get());

    //Create depth stencil state
    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state");
    

    //Draw Mask
    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc_drawMask(D3D11_DEFAULT);
    depthStencilDesc_drawMask.DepthEnable = false;
    depthStencilDesc_drawMask.StencilEnable = true;
    depthStencilDesc_drawMask.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	depthStencilDesc_drawMask.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_drawMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_drawMask.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

    depthStencilDesc_drawMask.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc_drawMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_drawMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
    depthStencilDesc_drawMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

    hr = device->CreateDepthStencilState(&depthStencilDesc_drawMask, depthStencilState_drawMask.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state");

    //Apply Mask
    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc_applyMask(D3D11_DEFAULT);
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc_applyMask.StencilEnable = true;
    depthStencilDesc_applyMask.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
    depthStencilDesc_applyMask.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_applyMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_applyMask.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

    depthStencilDesc_applyMask.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc_applyMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_applyMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc_applyMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

    hr = device->CreateDepthStencilState(&depthStencilDesc_applyMask, depthStencilState_applyMask.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state");



    //Viewport
    CD3D11_VIEWPORT viewport(0.0f,0.0f, windowWidth,windowHeight);
    deviceContext->RSSetViewports(1, &viewport);

    //Rasterizer State
    CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
    hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");

    //Wireframe Rasterizer State
    CD3D11_RASTERIZER_DESC rasterizerDescWireframe(D3D11_DEFAULT);
    rasterizerDescWireframe.FillMode = D3D11_FILL_WIREFRAME;
    hr = device->CreateRasterizerState(&rasterizerDescWireframe, rasterizerState_Wireframe.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");

    //Rasterizer State
    CD3D11_RASTERIZER_DESC rasterizerDescCullFront(D3D11_DEFAULT);
    rasterizerDescCullFront.CullMode = D3D11_CULL_FRONT;
    hr = device->CreateRasterizerState(&rasterizerDescCullFront, rasterizerState_CullFront.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");

    CD3D11_RASTERIZER_DESC rasterizerDescCullFrontWireframe(D3D11_DEFAULT);
	rasterizerDescCullFrontWireframe.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDescCullFrontWireframe.CullMode = D3D11_CULL_FRONT;
    hr = device->CreateRasterizerState(&rasterizerDescCullFrontWireframe, rasterizerState_Wireframe_CullFront.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state");

    //Blend State
    D3D11_BLEND_DESC blendDesc = { 0 };

    D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };

    rtbd.BlendEnable = true;
    rtbd.BlendOp = D3D11_BLEND_OP_ADD;
    rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
    rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
    rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.RenderTarget[0] = rtbd;

    hr = device->CreateBlendState(&blendDesc, blendState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create blend sate");

    //Font Batch
    spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext.Get());
    spriteFont = std::make_unique<DirectX::SpriteFont>(device.Get(), L"Data\\Fonts\\comic_sans_ms.spritefont");

    //Sampler State
    CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
    COM_ERROR_IF_FAILED(hr, "Failed to create sampler state");
    }
    catch (COMException& exception) {
        ErrorLogger::Log(exception);
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

    //3D Shaders 
    D3D11_INPUT_ELEMENT_DESC layout3D[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA ,0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    UINT numElements3D = ARRAYSIZE(layout3D);

    if (!vertexshader.Initialize(device, shaderfolder + L"vertexshader.cso", layout3D, numElements3D)) {
        return false;
    }

    if (!pixelshader.Initialize(device, shaderfolder + L"pixelshader.cso")) {
        return false;
    }

    if (!pixelshader_nolight.Initialize(device, shaderfolder + L"pixelshader_nolight.cso")) {
        return false;
    }

    //2D Shaders
    D3D11_INPUT_ELEMENT_DESC layout2D[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA ,0},
    };

	UINT numElements2D = ARRAYSIZE(layout2D);
    
    if (!vertexshader_2d.Initialize(device, shaderfolder + L"vertexshader_2d.cso", layout2D, numElements2D)) {
        return false;
    }

    if (!pixelshader_2d.Initialize(device, shaderfolder + L"pixelshader_2d.cso")) {
        return false;
    }
    
    return true;
}

bool Graphics::IntiializeScene()
{
    try {

        //Load Textures
        HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\DK.png", nullptr, kongTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file");

        hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\grass.jpg", nullptr, grassTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file");

        hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\brick.jpg", nullptr, brickTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file");

        hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\cowbase.png", nullptr, cowTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file");

        hr = DirectX::CreateWICTextureFromFile(device.Get(), L"Data\\Textures\\skybox.jpg", nullptr, skyboxTexture.GetAddressOf());
        COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file");

        //Initialize vertex shader constant buffer
        hr = cb_vs_vertexshader.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer");

        //Initialize pixel shader constant buffer
        hr = cb_ps_light.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer");

        hr = cb_vs_vertexshader_2d.Initialize(device.Get(), deviceContext.Get());
        COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer");

        cb_ps_light.data.ambientLightColour = XMFLOAT3(1, 1, 1);
        cb_ps_light.data.ambientLightStrength = 1.0f;

        //initialize models
        if (!carModel.Initialize("Data\\Objects\\Samples\\dodge_challenger.fbx", device.Get(), deviceContext.Get(), cb_vs_vertexshader))
            return false;

        //initialize models
        if (!nanosuitModel.Initialize("Data\\Objects\\nanosuit\\nanosuit.obj", device.Get(), deviceContext.Get(), cb_vs_vertexshader))
            return false;
        //initialize models
        if (!cowModel.Initialize("Data\\Objects\\cow.obj", device.Get(), deviceContext.Get(), cb_vs_vertexshader))
            return false;

        if (!light.Initialize(device.Get(), deviceContext.Get(), cb_vs_vertexshader))
            return false;

        if (!sprite.Initialize(this->device.Get(), this->deviceContext.Get(), 256, 256, "Data/Textures/DK.png", cb_vs_vertexshader_2d))
            return false;

        sprite.SetPosition(XMFLOAT3(windowWidth / 2 - sprite.GetWidth() / 2, windowHeight / 2 - sprite.GetHeight() / 2, 0.0f));
        Camera3D.SetPosition(0.0f, 0.0f, -2.0f);
        Camera3D.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.f);

        camera2D.SetProjectionValues(windowWidth, windowHeight, 0.0f, 1.0f);

        if (!cube.Initialize(this->device.Get(), this->deviceContext.Get(), cb_vs_vertexshader)) {
            return false;
        }

        if (!skyBox.Initialize(this->device.Get(), this->deviceContext.Get(), cb_vs_vertexshader)) {
            return false;
        }
    }
    catch (COMException & exception) {
        ErrorLogger::Log(exception);
        return false;
    }
    return true;
}