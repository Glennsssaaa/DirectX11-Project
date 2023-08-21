#pragma once
#include "Utility/AdapterReader.h"
#include "Shaders/Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include "GameObjects/Camera3D.h"
#include "..\\Utility\\Timer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "GameObjects/RenderableGameObject.h"
#include "GameObjects/Light.h"
#include "GameObjects/Camera2D.h"
#include "GameObjects/Sprite.h"
#include "GameObjects/Primitives/CubeMesh.h"
#include ".\GameObjects\PrimitiveGameObject.h"

class Graphics
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera3D Camera3D;
	Camera2D camera2D;
	RenderableGameObject carModel;
	RenderableGameObject nanosuitModel;
	RenderableGameObject cowModel;
	Light light;
	Sprite sprite;
	CubeMesh cube;
private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool IntiializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexshader;
	PixelShader pixelshader;
	PixelShader pixelshader_nolight;

	PixelShader pixelshader_2d;
	VertexShader vertexshader_2d;

	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_light> cb_ps_light;
	ConstantBuffer<CB_VS_vertexshader_2d> cb_vs_vertexshader_2d;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_drawMask;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_applyMask;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_Wireframe;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_Wireframe_CullFront;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> kongTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> brickTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cowTexture;

	int windowWidth = 0;
	int windowHeight = 0;

	Timer fpsTimer;
	bool enableFlashlight;
};

