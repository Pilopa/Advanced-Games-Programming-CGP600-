#pragma once

#include "Collider.h"
#include "Collision.h"
#include "CollisionManager.h"

void Collider::awake() {
	CollisionManager::instance()->registerCollider(this);
}

void Collider::shutdown()
{
	CollisionManager::instance()->deregisterCollider(this);
}

void Collider::update() {
	// Do nothing
}