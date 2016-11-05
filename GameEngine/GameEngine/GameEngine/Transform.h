#pragma once

#include "Component.h"

struct Position {
	float x = 0.0F, 
		y = 0.0F, 
		z = 0.0F;
};

struct Rotation {
	float x = 0.0F,
		y = 0.0F,
		z = 0.0F;
};

struct Scale {
	float x = 0.0F,
		y = 0.0F,
		z = 0.0F;
};

class Transform : public Component {

	public:
		Transform();

		const Position position;
		const Rotation rotation;
		const Scale scale;

		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
};