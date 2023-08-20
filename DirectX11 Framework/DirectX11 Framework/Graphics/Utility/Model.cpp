#include "Model.h"

bool Model::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
    this->device = device;
    this->deviceContext = deviceContext;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;

	try {
		if(!LoadModel(filepath))
			return false;
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
    return true;
}


void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader->GetAddressOf());

	for (int i = 0; i < meshes.size(); i++)
	{
		cb_vs_vertexshader->data.wvpMatrix = meshes[i].GetTransformMatrix() * worldMatrix * viewProjectionMatrix;
		cb_vs_vertexshader->data.worldMatrix = meshes[i].GetTransformMatrix() * worldMatrix;
		cb_vs_vertexshader->ApplyChanges();
		meshes[i].Draw();
	}
}

bool Model::LoadModel(const std::string& filepath)
{
	directory = StringHelper::GetDirectoryFromPath(filepath);
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (pScene == nullptr) {
		return false;
	}
	ProcessNode(pScene->mRootNode, pScene, XMMatrixIdentity());
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
{
	XMMATRIX transformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, transformMatrix));
	}
	for (UINT i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene, transformMatrix);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
	
	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}
		
		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(device, deviceContext, vertices, indices, textures, transformMatrix);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storeType = TextureStorageType::INVALID;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);

	if (textureCount == 0) {
		storeType = TextureStorageType::NONE;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType) {
		case aiTextureType_DIFFUSE:
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack()) {
				materialTextures.push_back(Texture(device, Colours::UnloadedTextureColour, textureType));
				return materialTextures;
			}
			materialTextures.push_back(Texture(device, Colour(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
			return materialTextures;
		}
	}
	else{
		for (UINT i = 0; i < textureCount; i++) {
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			TextureStorageType storeType = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
			switch (storeType) {
			case TextureStorageType::EMBEDDED_COMPRESSED:
			{
				const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				Texture embeddedTexture(device,
					reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth,
					textureType);
				materialTextures.push_back(embeddedTexture);
				break;
			}
			case TextureStorageType::EMBEDDED_INDEX_COMPRESSED: {
				int index = GetTextureIndex(&path);
				Texture embeddedIndexedTexture(device,
					reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
					pScene->mTextures[index]->mWidth,
					textureType);
				materialTextures.push_back(embeddedIndexedTexture);
				break;
			}
			case TextureStorageType::DISK: {
				std::string filename = directory + '\\' + path.C_Str();
				Texture diskTexture(device, filename, textureType);
				materialTextures.push_back(diskTexture);
				break;
			}
			}
		}
	}

	if (materialTextures.size() == 0) {
		materialTextures.push_back(Texture(device, Colours::UnhandledTextureColour, aiTextureType::aiTextureType_DIFFUSE));
	}
	return materialTextures;
}

int Model::GetTextureIndex(aiString * pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMaterial, unsigned int i, aiTextureType textureType)
{
	if (pMaterial->GetTextureCount(textureType) == 0)
		return TextureStorageType::NONE;
	
	aiString path;
	pMaterial->GetTexture(textureType, i, &path);
	std::string texturePath = path.C_Str();
	if (texturePath[0] == '*') {
		if (pScene->mTextures[0]->mHeight == 0)
			return TextureStorageType::EMBEDDED_INDEX_COMPRESSED;
		else
			assert("Support for non-compressed embedded textures not implemented");
			return TextureStorageType::EMBEDDED_INDEX_NON_COMPRESSED;
	}

	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str())) {
		if (pTex->mHeight == 0) {
			return TextureStorageType::EMBEDDED_COMPRESSED;
		}
		else {
			assert("support for non compressed embedded does not exist ");
			return TextureStorageType::EMBEDDED_NON_COMPRESSED;
		}
	}

	if (texturePath.find('.') != std::string::npos)
		return TextureStorageType::DISK;
	
	return TextureStorageType::NONE;
}
