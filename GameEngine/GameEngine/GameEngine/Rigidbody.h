#pragma once

#include <DirectXMath.h>
#include "Component.h"
#include "Utility.h"

using namespace DirectX;

#define GRAVITY {0.0f, -9.81f, 0.0f, 1.0f}

class Rigidbody : public Component {

	public:

		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void shutdown(); // Overrides Component::shutdown
		
		XMVECTOR getVelocity();
		XMVECTOR getTimestepVelocity();
		void addForce(XMVECTOR force);
		void reset();
		void reset(XMVECTOR axis);

	private:
		XMVECTOR velocity = ZERO_VECTOR;

		void tick(); // One physics tick

};