#pragma once
#include "../PrimitiveGameObject.h"
class CubeMesh : public PrimitiveGameObject
{
public:
	int GetIndexCount();
	int indexCount;
	ID3D11Buffer* vertexBuffer, * indexBuffer;

protected:
	ReturnedData CalculateShape() override;

	int vertexCount;
	int resolution;
};

