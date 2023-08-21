#pragma once
#include "../PrimitiveGameObject.h"
class PlaneMesh : public PrimitiveGameObject
{
public:
	int indexCount;

protected:
	ReturnedData CalculateShape() override;

	int vertexCount;
	int resolution;
};

