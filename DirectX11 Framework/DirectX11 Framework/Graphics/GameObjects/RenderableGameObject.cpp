#include "RenderableGameObject.h"

bool RenderableGameObject::Initialize(const std::string& filepath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	if (!model.Initialize(filepath, device, deviceContext, cb_vs_vertexshader))
	{
		return false;
	}

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(1.0f, 1.0f, 1.0f);
	UpdateMatrix();
	return true;
}

void RenderableGameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	model.Draw(worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);
	UpdateDirectionVectors();
	worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * worldMatrix;
}
