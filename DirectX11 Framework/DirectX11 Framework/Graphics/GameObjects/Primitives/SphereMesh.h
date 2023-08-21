#pragma once
#include "../PrimitiveGameObject.h"
class SphereMesh : public PrimitiveGameObject
{
public:
	int indexCount;

protected:
	ReturnedData CalculateShape() override;

	int vertexCount;
	int resolution;
};

