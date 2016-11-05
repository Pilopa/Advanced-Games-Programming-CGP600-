#pragma once

#include "Component.h"

class Collider : public Component {

	public: 
		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake

};