#pragma once

#include "Transform.h"
#include "GameObject.h"

Transform::Transform()
{

}

void Transform::translate(DirectX::XMVECTOR vector)
{
	localPosition->x += vector.m128_f32[0];
	localPosition->y += vector.m128_f32[1];
	localPosition->z += vector.m128_f32[2];
}

void Transform::rotate(Axis axis, float degree)
{
	switch (axis) {
		case AXIS_X:
			localRotation->pitch += degree;
			break;
		case AXIS_Y:
			localRotation->yaw += degree;
			break;
		case AXIS_Z:
			localRotation->roll += degree;
			break;
	}
}

void Transform::scale(DirectX::XMVECTOR factor)
{
	localScale->x += factor.m128_f32[0];
	localScale->y += factor.m128_f32[1];
	localScale->z += factor.m128_f32[2];
}

DirectX::XMVECTOR Transform::getDirectionalVector()
{
	return getRotationMatrix().r[2];
}

DirectX::XMVECTOR Transform::getPositionVector()
{
	return { localPosition->x, localPosition->y, localPosition->z };
}

DirectX::XMVECTOR Transform::getRotationVector()
{
	return{ localRotation->roll, localRotation->pitch, localRotation->yaw };
}

DirectX::XMVECTOR Transform::getScaleVector()
{
	return{ localScale->x, localScale->y, localScale->z };
}

DirectX::XMMATRIX Transform::getTranslationMatrix()
{
	return DirectX::XMMatrixTranslation(localPosition->x, localPosition->y, localPosition->z);
}

DirectX::XMMATRIX Transform::getRotationMatrix()
{
	return DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(localRotation->pitch), DirectX::XMConvertToRadians(localRotation->yaw), DirectX::XMConvertToRadians(localRotation->roll));
}

DirectX::XMMATRIX Transform::getScaleMatrix()
{
	return DirectX::XMMatrixScaling(localScale->x, localScale->y, localScale->z);
}

DirectX::XMMATRIX Transform::getWorldMatrix()
{
	DirectX::XMMATRIX local = getScaleMatrix() * getRotationMatrix() * getTranslationMatrix();
	GameObject* parent = nullptr;
	if ((parent = this->getGameObject()->getParent()) != nullptr) {
		local *= parent->getTransform()->getWorldMatrix();
	}
	return local;
}

void Transform::update()
{
	// Do nothing
}

void Transform::awake()
{
	// Do nothing
}

void Transform::shutdown()
{
	// Do nothing
}

DirectX::XMVECTOR Transform::forward()
{
	return { 0.0F, 0.0F, 1.0F };
}

DirectX::XMVECTOR Transform::up()
{
	return { 0.0F, 1.0F, 0.0F };
}

DirectX::XMVECTOR Transform::right()
{
	return { 1.0F, 0.0F, 0.0F };
}
