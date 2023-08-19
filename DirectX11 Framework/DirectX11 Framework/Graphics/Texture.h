#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Colour.h"
#include <assimp/material.h>
#include "..\\Utility\\ErrorLogger.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

enum class TextureStorageType {
	INVALID,
	NONE,
	EMBEDDED_INDEX_COMPRESSED,
	EMBEDDED_INDEX_NON_COMPRESSED,
	EMBEDDED_COMPRESSED,
	EMBEDDED_NON_COMPRESSED,
	DISK
};

class Texture
{
public:
	Texture(ID3D11Device* device, const Colour& colour, aiTextureType type);
	Texture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type);
	Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type);
	aiTextureType GetType();
	ID3D11ShaderResourceView* GetTextureResourceView();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();
private:
	void Initialize1x1ColourTexture(ID3D11Device* device, const Colour& colourData, aiTextureType type);
	void InitializeColourTexture(ID3D11Device* device, const Colour* colourData, UINT width, UINT height, aiTextureType type);
	Microsoft::WRL::ComPtr<ID3D11Resource> texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
};

