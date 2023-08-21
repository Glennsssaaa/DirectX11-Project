#pragma once
#include "../PrimitiveGameObject.h"
class CubeMesh : public PrimitiveGameObject
{
public:
	int indexCount;

protected:
	ReturnedData CalculateShape() override;

	int vertexCount;
	int resolution;
};

