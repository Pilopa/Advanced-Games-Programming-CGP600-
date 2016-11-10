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

		virtual void update(); // Overrides Component::update
		virtual void awake(); // Overrides Component::update
		
		Camera(PROJECTION_TYPE projectionType, float fov_angle, float near_z, float far_z, float size);
		DirectX::XMMATRIX getViewMatrix();
		DirectX::XMMATRIX getProjectionMatrix(float aspectRatio);

};