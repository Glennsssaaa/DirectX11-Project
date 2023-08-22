#pragma once
#include "GameObject2D.h"
#include "../Shaders/ConstantBuffer.h"
#include "../Shaders/IndexBuffer.h"
#include "../Shaders/VertexBuffer.h"
#include "../Shaders/Vertex.h"
#include "../Utility/Texture.h"
#include <string>
class OrthographicPlane : public GameObject2D
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float height, ConstantBuffer<CB_VS_vertexshader_2d>& cb_vs_vertexshader_2d);
	void Draw(XMMATRIX orthoMatrix);
	float GetWidth();
	float GetHeight();
	DirectX::XMMATRIX orthoMatrix;

private:
	void UpdateMatrix() override;

	ConstantBuffer<CB_VS_vertexshader_2d>* cb_vs_vertexshader_2d = nullptr;
	XMMATRIX worldMatrix = XMMatrixIdentity();

	std::unique_ptr<Texture> texture;
	ID3D11DeviceContext* deviceContext = nullptr;

	IndexBuffer indices;
	VertexBuffer<Vertex2D> vertices;
};

