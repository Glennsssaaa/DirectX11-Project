#pragma once
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class Camera : public GameObject
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix()const;
	const XMMATRIX& GetProjectionMatrix()const;

private:
	void UpdateMatrix();

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

};

