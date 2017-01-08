#pragma once

#include "CustomScript.h"

class FreeFlightCameraScript : public CustomScript {

	void update(); // Overrides Component::update
	void awake(); // Overrides Component::awake
	void shutdown(); // Overrides Component::shutdown
	void onCollision(Collider * first, Collision * collision); // Overrides CustomScript::onCollision

};