#pragma once

#include "Camera.h"
#include "ApplicationManager.h"
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

DirectX::XMMATRIX Camera::get2DProjectionMatrix()
{
	RECT bounds;
	ApplicationManager::instance()->getWindow()->getBounds(&bounds);
	long width = abs(bounds.right - bounds.left);
	long height = abs(bounds.bottom - bounds.top);

	if (width == 0 || height == 0) return DirectX::XMMatrixIdentity();
	else return DirectX::XMMatrixOrthographicLH((float) width, (float) height, near_z, far_z);
}

DirectX::XMMATRIX Camera::getProjectionMatrix(float aspectRatio)
{
	return getProjectionMatrix(projectionType, aspectRatio);
}

DirectX::XMMATRIX Camera::getProjectionMatrix(PROJECTION_TYPE projectionType, float aspectRatio)
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

void Camera::shutdown()
{
	// Do nothing
}
