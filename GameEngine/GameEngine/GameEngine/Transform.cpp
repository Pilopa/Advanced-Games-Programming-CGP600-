#pragma once

#include "Transform.h"

Transform::Transform()
{

}

void Transform::translate(DirectX::XMVECTOR vector)
{
	localPosition->x += vector.m128_f32[0];
	localPosition->y += vector.m128_f32[1];
	localPosition->z += vector.m128_f32[2];
}

DirectX::XMVECTOR Transform::getDirectionalVector()
{
	return Transform::forward(); // TODO: Implement correctly
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
	return getScaleMatrix() * getRotationMatrix() * getTranslationMatrix();
}

void Transform::update()
{
	// Do nothing
}

void Transform::awake()
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
