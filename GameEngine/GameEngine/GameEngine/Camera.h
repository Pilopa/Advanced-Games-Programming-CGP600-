#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Component.h"

enum PROJECTION_TYPE {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class Camera : public Component {

	private:

		PROJECTION_TYPE projectionType;

		float fov_angle = 0.0,
			near_z = 0.0,
			far_z = 0.0,
			size = 0.0;

	public:

		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void shutdown(); // Overrides Component::shutdown
		
		Camera(PROJECTION_TYPE projectionType, float fov_angle, float near_z, float far_z, float size);
		DirectX::XMMATRIX getViewMatrix();
		DirectX::XMMATRIX get2DProjectionMatrix();
		DirectX::XMMATRIX getProjectionMatrix(float aspectRatio);
		DirectX::XMMATRIX getProjectionMatrix(PROJECTION_TYPE projectionType, float aspectRatio);

};