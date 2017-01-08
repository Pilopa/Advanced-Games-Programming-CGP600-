#pragma once

#include "ProjectileScript.h"
#include "Rigidbody.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"

void ProjectileScript::awake()
{
	Rigidbody* rigidbody = getGameObject()->getComponent <Rigidbody>();
	if (rigidbody) {
		// Forward vector
		auto forward = getGameObject()->getTransform()->getWorldDirectionalVector();

		// Add the force forward
		rigidbody->addForce(XMVectorMultiply(forward, Vector(PROJECTILE_SPEED_FORWARD)));

		// Add force upwards
		rigidbody->addForce(XMVectorMultiply(Transform::up(), Vector(PROJECTILE_SPEED_UPWARD)));
	}
}

void ProjectileScript::update()
{
	// Destroy projectile if it somehow managed to fly under ground
	if (getGameObject()->getTransform()->getWorldPositionVector().m128_f32[1] <= -1.0f) {
		getGameObject()->destroy();
	}
}

void ProjectileScript::onCollision(Collider * first, Collision * collision)
{
	// Destroy projectile
	getGameObject()->destroy();
}
