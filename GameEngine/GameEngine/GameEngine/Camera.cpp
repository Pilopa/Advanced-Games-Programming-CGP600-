#pragma once

#include "Camera.h"
#include "Debug.h"
#include "GameObject.h"

Camera::Camera(PROJECTION_TYPE projectionType, float fov_angle, float near_z, float far_z, float size)
{
	this->projectionType = projectionType;
	this->fov_angle = fov_angle;
	this->near_z = near_z;
	this->far_z = far_z;
	this->size = size;
}

DirectX::XMMATRIX Camera::getViewMatrix()
{
	return DirectX::XMMatrixInverse(nullptr, getGameObject()->getTransform()->getWorldMatrix());
}

DirectX::XMMATRIX Camera::getProjectionMatrix(float aspectRatio)
{
	switch (projectionType) {
		case PERSPECTIVE:
			return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov_angle), aspectRatio, near_z, far_z);
			break;
		case ORTHOGRAPHIC:
			return DirectX::XMMatrixOrthographicLH(size, size * aspectRatio, near_z, far_z);
			break;
		default:
			LogError("Unknown camera projection type!");
			return DirectX::XMMatrixIdentity();
	}
}

void Camera::update()
{
	// Do nothing
}

void Camera::awake()
{
	// Do nothing
}