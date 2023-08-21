#pragma once
#include "GameObject3D.h"

struct ReturnedData {
	std::vector<Vertex3D> Vertices;
	std::vector<DWORD> Indices;
};

class PrimitiveGameObject : public GameObject3D
{
	
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	void Draw(const XMMATRIX& viewProjectionMatrix);

protected:
	void UpdateMatrix() override;
	virtual ReturnedData CalculateShape();

	XMMATRIX worldMatrix = XMMatrixIdentity();
private:
	
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cb_vs_vertexshader = nullptr;
	VertexBuffer<Vertex3D> vertexBuffer;
	IndexBuffer indexBuffer;
	std::vector<Mesh> meshes;
};