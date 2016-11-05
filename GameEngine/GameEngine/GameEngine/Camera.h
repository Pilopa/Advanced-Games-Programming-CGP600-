#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class Camera {

	private:

		float m_x,
			m_y,
			m_z,
			m_camera_rotation,
			m_dx,
			m_dz;
		DirectX::XMVECTOR m_position,
			m_lookat,
			m_up;

		void Camera::Recalculate();

	public:
		Camera(float x, float y, float z, float camera_rotation);
		void Rotate(float degrees);
		void Forward(float distance);
		DirectX::XMMATRIX GetViewMatrix();

};