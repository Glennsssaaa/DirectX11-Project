#pragma once
#include "..\Shaders\\Vertex.h"
#include "..\Shaders\\VertexBuffer.h"
#include "..\Shaders\\IndexBuffer.h"
#include "..\Shaders\\ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& mesh);
	void Draw();
	const DirectX::XMMATRIX& GetTransformMatrix();
private:
	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indexBuffer;
	ID3D11DeviceContext* deviceContext;
	std::vector<Texture> textures;
	DirectX::XMMATRIX transformMatrix;
};

