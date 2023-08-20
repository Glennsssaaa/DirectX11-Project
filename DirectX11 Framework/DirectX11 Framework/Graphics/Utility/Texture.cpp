#include "Texture.h"

Texture::Texture(ID3D11Device* device, const Colour& colour, aiTextureType type)
{
	Initialize1x1ColourTexture(device, colour, type);
}

Texture::Texture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type)
{
	InitializeColourTexture(device, colourData, width, height, type);
}

Texture::Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type)
{
	this->type = type;
	if (StringHelper::GetFileExtension(filePath) == ".dds") {
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
		if (FAILED(hr)) {
			Initialize1x1ColourTexture(device, Colours::UnloadedTextureColour, type);
		}
		return;
	}
	else {
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), texture.GetAddressOf(), textureView.GetAddressOf());
		if (FAILED(hr)){
			Initialize1x1ColourTexture(device, Colours::UnloadedTextureColour, type);
		}
		return;
	}
}

Texture::Texture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type)
{
	this->type = type;
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, texture.GetAddressOf(), textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create texture from memory");
}

aiTextureType Texture::GetType()
{
	return type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
	return textureView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
	return textureView.GetAddressOf();
}

void Texture::Initialize1x1ColourTexture(ID3D11Device* device, const Colour& colourData, aiTextureType type)
{
	InitializeColourTexture(device, &colourData, 1, 1, type);
}

void Texture::InitializeColourTexture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type)
{
	this->type = type;
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colourData;
	initialData.SysMemPitch = width * sizeof(Colour);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	COM_ERROR_IF_FAILED(hr, "Failed to create texture from colour data");
	texture = static_cast<ID3D11Resource*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture");
}
