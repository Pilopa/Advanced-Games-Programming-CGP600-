#include "camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_camera_rotation = camera_rotation;
	m_up = DirectX::XMVectorSet(0.0F, 1.0F, 0.0F, 0.0F);
	Recalculate();
}

void Camera::Recalculate() {
	m_dx = sin(m_camera_rotation * (DirectX::XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (DirectX::XM_PI / 180.0));
	m_position = DirectX::XMVectorSet(m_x, m_y, m_z, 0.0F);
	m_lookat = DirectX::XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0F);
}

void Camera::Rotate(float degrees)
{
	m_camera_rotation += degrees;
	Recalculate();
}

void Camera::Forward(float distance)
{
	m_x += m_dx * distance;
	m_z += m_dz * distance;
	Recalculate();
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return DirectX::XMMatrixLookAtLH(m_position, m_lookat, m_up);
}

