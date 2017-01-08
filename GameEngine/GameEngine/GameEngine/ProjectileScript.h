#pragma once

#include "CustomScript.h"

#define PROJECTILE_SPEED_FORWARD 20.0f
#define PROJECTILE_SPEED_UPWARD 1.0f

class ProjectileScript : public CustomScript {

	public:
		void awake(); // Overrides CustomScript::awake
		void update(); // Overrides CustomScript::update
		void onCollision(Collider * first, Collision * collision); // Overrides CustomScript::onCollision

};