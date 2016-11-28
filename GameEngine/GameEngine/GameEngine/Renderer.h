#pragma once

#include "Component.h"

class Renderer : public Component {

	public:
		virtual void update() = 0; // Overrides Component::update
		virtual void draw() = 0;
		virtual void shutdown(); // Overrides Component::shutdown
		virtual void awake(); // Overrides Component::awake
		
};