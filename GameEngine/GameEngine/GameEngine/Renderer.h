#pragma once

#include "Component.h"

class Renderer : public Component {

	public:
		virtual void update() = 0; // Overrides Component::update
		virtual void awake(); // Overrides Component::awake
		virtual void draw() = 0;

};