#include "PrimitiveGameObject.h"

bool PrimitiveGameObject::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->cb_vs_vertexshader = &cb_vs_vertexshader;
	
	ReturnedData shapeData = CalculateShape();
	std::vector<Vertex3D> vertices = shapeData.Vertices;
	std::vector<DWORD> indices = shapeData.Indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	HRESULT hr = vertexBuffer.Initialize(this->device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for mesh.");
	
	hr = indexBuffer.Initialize(this->device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer for mesh.");
	
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	SetScale(5.0f, 5.0f, 5.0f);
	

	std::vector<Texture> textures;
	Mesh mesh = Mesh(this->device, this->deviceContext, vertices, indices, textures, XMMatrixIdentity());
	meshes.push_back(mesh);
	UpdateMatrix();
	
	return true;
}
ReturnedData PrimitiveGameObject::CalculateShape()
{
	return ReturnedData();
}

void PrimitiveGameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader->GetAddressOf());
	cb_vs_vertexshader->data.wvpMatrix = meshes[0].GetTransformMatrix() * worldMatrix * viewProjectionMatrix;
	cb_vs_vertexshader->data.worldMatrix = meshes[0].GetTransformMatrix() * worldMatrix;
	cb_vs_vertexshader->ApplyChanges();
	
	UINT offset = 0;
	
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(indexBuffer.indexCount(), 0, 0);
}

void PrimitiveGameObject::UpdateMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);
	UpdateDirectionVectors();
	worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z) * worldMatrix;
}
