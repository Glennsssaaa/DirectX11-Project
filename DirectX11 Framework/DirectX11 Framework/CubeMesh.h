#pragma once
#include "Graphics/GameObjects/RenderableGameObject.h"
class CubeMesh : public RenderableGameObject
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader, int resolution = 20);
	int GetIndexCount();
	int indexCount;
	ID3D11Buffer* vertexBuffer, * indexBuffer;

protected:
	void initBuffers(ID3D11Device* device);

	int vertexCount;
	int resolution;
};

