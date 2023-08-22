#include "OrthographicPlane.h"

bool OrthographicPlane::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float width, float height, ConstantBuffer<CB_VS_vertexshader_2d>& cb_vs_vertexshader_2d)
{
	this->deviceContext = deviceContext;
	if (deviceContext == nullptr) {
		return false;
	}

	this->cb_vs_vertexshader_2d = &cb_vs_vertexshader_2d;

	std::vector<Vertex2D> vertexData = {
		Vertex2D(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f), //TopLeft
		Vertex2D(0.5f, -0.5f, 0.0f, 1.0f, 0.0f), //TopRight
		Vertex2D(-0.5, 0.5, 0.0f, 0.0f, 1.0f), //Bottom Left
		Vertex2D(0.5, 0.5, 0.0f, 1.0f, 1.0f), //Bottom Right
	};

	std::vector<DWORD> indexData = {
		0,1,2,
		2,1,3,
	};

//	texture = std::make_unique<Texture>(device, "Data\\Textures\\skybox.jpg", aiTextureType::aiTextureType_DIFFUSE);

	HRESULT hr = vertices.Initialize(device, vertexData.data(), vertexData.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for sprite.");

	hr = indices.Initialize(device, indexData.data(), indexData.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for sprite.");

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(width, height, 1.0f);

	orthoMatrix = XMMatrixOrthographicLH(width, height, 0.0f, 1.0f);

	return true;
}

void OrthographicPlane::Draw(XMMATRIX orthoMatrix)
{
	XMMATRIX wvpMatrix = worldMatrix * orthoMatrix;
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader_2d->GetAddressOf());
	cb_vs_vertexshader_2d->data.wvpMatrix = wvpMatrix;
	cb_vs_vertexshader_2d->ApplyChanges();

	//deviceContext->PSSetShaderResources(0, 1, texture->GetTextureResourceViewAddress());

	const UINT offsets = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertices.GetAddressOf(), vertices.StridePtr(), &offsets);
	deviceContext->IASetIndexBuffer(indices.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(indices.indexCount(), 0, 0);
}

float OrthographicPlane::GetWidth()
{
	return scale.x;
}

float OrthographicPlane::GetHeight()
{
	return scale.y;
}

void OrthographicPlane::UpdateMatrix()
{
	worldMatrix = XMMatrixScaling(scale.x, scale.y, 1.0f) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x + scale.x / 2.0f, pos.y + scale.y / 2.0f, pos.z);
}
