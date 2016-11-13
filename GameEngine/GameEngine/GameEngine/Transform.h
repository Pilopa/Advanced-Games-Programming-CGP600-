#pragma once

#include <DirectXMath.h>
#include "Component.h"

enum Axis {
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

struct Position {
	float x = 0.0F, 
		y = 0.0F, 
		z = 0.0F;
};

// Euler Angles
struct Rotation {

	// Rotation around z-Axis
	float roll = 0.0F;

	// Rotation around x-Axis
	float pitch = 0.0F;

	// Rotation around y-Axis
	float yaw = 0.0F;
};

struct Scale {
	float x = 1.0F,
		y = 1.0F,
		z = 1.0F;
};

class Transform : public Component {

	public:
		Transform();

		Position * localPosition = new Position();
		Rotation * localRotation = new Rotation();
		Scale * localScale = new Scale();

		void translate(DirectX::XMVECTOR vector);
		void rotate(Axis axis, float degree);
		void scale(DirectX::XMVECTOR factor);

		DirectX::XMVECTOR getDirectionalVector();

		DirectX::XMVECTOR getPositionVector();
		DirectX::XMVECTOR getRotationVector();
		DirectX::XMVECTOR getScaleVector();

		DirectX::XMMATRIX getTranslationMatrix();
		DirectX::XMMATRIX getRotationMatrix();
		DirectX::XMMATRIX getScaleMatrix();
		DirectX::XMMATRIX getWorldMatrix();

		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake

		// Returns the world's directional vector
		static DirectX::XMVECTOR forward();

		// Returns the world's up vector
		static DirectX::XMVECTOR up();

		// Returns the world's right vector
		static DirectX::XMVECTOR right();
};