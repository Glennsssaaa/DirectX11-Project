#include "GameObject.h"

const XMVECTOR& GameObject::GetPositionVector() const
{
	return posVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const
{
	return pos;
}

const XMVECTOR& GameObject::GetRotationVector() const
{
	return rotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const
{
	return rot;
}

void GameObject::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	posVector = pos;
	UpdateMatrix();
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	posVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::SetScale(const XMVECTOR& scale)
{
	XMStoreFloat3(&this->scale, scale);
	scaleVector = scale;
	UpdateMatrix();
}

void GameObject::SetScale(const XMFLOAT3& scale)
{
	this->scale = scale;
	scaleVector = XMLoadFloat3(&scale);
	UpdateMatrix();
}

void GameObject::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
	scaleVector = XMLoadFloat3(&scale);
	UpdateMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR& pos)
{
	posVector += pos;
	XMStoreFloat3(&this->pos, posVector);
	UpdateMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	posVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::SetRotation(const XMVECTOR& rot)
{
	XMStoreFloat3(&this->rot, rot);
	rotVector = rot;
	UpdateMatrix();
}

void GameObject::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	rotVector = XMLoadFloat3(&rot);
	UpdateMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	rot = XMFLOAT3(x, y, z);
	rotVector = XMLoadFloat3(&pos);
	UpdateMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR& rot)
{
	rotVector += rot;
	XMStoreFloat3(&this->rot, rotVector);
	UpdateMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	rotVector = XMLoadFloat3(&rot);
	UpdateMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = XMLoadFloat3(&rot);
	UpdateMatrix();
}

void GameObject::UpdateMatrix()
{
	assert("Must be Overridenc");
}