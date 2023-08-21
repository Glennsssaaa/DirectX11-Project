#pragma once
#include "..\\Graphics\\Utility\\Mesh.h"

using namespace DirectX;

class Model
{
public:
	bool Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);
	std::vector<Mesh> meshes;
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix);
private:
	bool LoadModel(const std::string& filepath);
	void ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMaterial, unsigned int i, aiTextureType textureType);
	int GetTextureIndex(aiString* pStr);
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cb_vs_vertexshader = nullptr;

	std::string directory = "";
};

