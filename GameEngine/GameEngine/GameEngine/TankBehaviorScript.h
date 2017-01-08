#pragma once

#include "CustomScript.h"

#define DEFAULT_MOVE_SPEED 5.0f
#define DEFAULT_TURN_SPEED 45.0f
#define DEFAULT_MAX_HEALTH 100.0f
#define DEFAULT_RELOAD_TIME 1.0f

class TankBehaviorScript : public CustomScript {

	public:
		virtual void update(); // Overrides CustomScript::update

	protected:
		float moveSpeed = DEFAULT_MOVE_SPEED;
		float turnSpeed = DEFAULT_TURN_SPEED;
		float maxHealth = DEFAULT_MAX_HEALTH;
		float health = maxHealth;
		float reloadTimer = DEFAULT_RELOAD_TIME;

		void shoot();
};