#pragma once

#include "Component.h"

class CustomScript : public Component {

	public:

		virtual void update(); // Overrides Component::update
		virtual void awake(); // Overrides Component::awake
		virtual void shutdown(); // Overrides Component::shutdown

		// Called when one of the associated game object's colliders colliders with another
		virtual void onCollision(Collider* first, Collision* collision);

};