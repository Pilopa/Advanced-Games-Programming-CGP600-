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

DirectX::XMVECTOR Transform::getLocalDirectionalVector()
{
	return getLocalRotationMatrix().r[2];
}

DirectX::XMVECTOR Transform::getLocalPositionVector()
{
	return { localPosition->x, localPosition->y, localPosition->z };
}

DirectX::XMVECTOR Transform::getLocalRotationVector()
{
	return{ localRotation->roll, localRotation->pitch, localRotation->yaw };
}

DirectX::XMVECTOR Transform::getLocalScaleVector()
{
	return{ localScale->x, localScale->y, localScale->z };
}

DirectX::XMMATRIX Transform::getLocalTranslationMatrix()
{
	return DirectX::XMMatrixTranslation(localPosition->x, localPosition->y, localPosition->z);
}

DirectX::XMMATRIX Transform::getLocalRotationMatrix()
{
	return DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(localRotation->pitch), DirectX::XMConvertToRadians(localRotation->yaw), DirectX::XMConvertToRadians(localRotation->roll));
}

DirectX::XMMATRIX Transform::getLocalScaleMatrix()
{
	return DirectX::XMMatrixScaling(localScale->x, localScale->y, localScale->z);
}

DirectX::XMMATRIX Transform::getWorldMatrix()
{
	DirectX::XMMATRIX local = getLocalScaleMatrix() * getLocalRotationMatrix() * getLocalTranslationMatrix();
	GameObject* parent = nullptr;
	if ((parent = this->getGameObject()->getParent()) != nullptr) {
		local *= parent->getTransform()->getWorldMatrix();
	}
	return local;
}

DirectX::XMVECTOR Transform::getWorldPositionVector()
{
	DirectX::XMVECTOR local = getLocalPositionVector();
	GameObject* parent = nullptr;
	if ((parent = this->getGameObject()->getParent()) != nullptr) {
		local = DirectX::XMVectorAdd(local, parent->getTransform()->getLocalPositionVector());
	}
	return local;
}

DirectX::XMVECTOR Transform::getWorldDirectionalVector()
{
	return getWorldRotationMatrix().r[2];
}

DirectX::XMMATRIX Transform::getWorldRotationMatrix()
{
	// Initialize with local values
	float roll = localRotation->roll,
		pitch = localRotation->pitch,
		yaw = localRotation->yaw;

	// Add parent values
	GameObject* parent = nullptr;
	if ((parent = this->getGameObject()->getParent()) != nullptr) {
		roll += parent->getTransform()->localRotation->roll;
		pitch += parent->getTransform()->localRotation->pitch;
		yaw += parent->getTransform()->localRotation->yaw;
	}

	// Clamp to 360°, calculation is minimally unprecise due to int conversion
	roll = (int) roll % 360;
	pitch = (int) pitch % 360;
	yaw = (int) yaw % 360;

	// Return matrix
	return DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(pitch), DirectX::XMConvertToRadians(yaw), DirectX::XMConvertToRadians(roll));
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
