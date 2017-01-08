#pragma once

#include "CollisionManager.h"
#include "Collider.h"
#include "Collision.h"
#include "GameObject.h"
#include "Transform.h"
#include "Utility.h"
#include "Debug.h"

CollisionManager* CollisionManager::s_instance = nullptr;

void CollisionManager::performCollisionCheck()
{

	for (std::map<Collider*, DirectX::XMMATRIX>::const_iterator iterator = colliders.begin(), end = colliders.end(); iterator != end; ++iterator) {
		std::pair<Collider*, DirectX::XMMATRIX> pair = *iterator;

		// Check if collider moved
		DirectX::XMMATRIX currentPos = pair.first->getGameObject()->getTransform()->getWorldMatrix();
		if (!areEqual(&pair.second, &currentPos)) {

			// If it did, check collision for it
			forCollider(pair.first);

			// Then update its position to the new current one
			colliders.insert_or_assign(pair.first, currentPos);
		}
	}
}

void CollisionManager::add(Collider * collider)
{
	// Do initial collision check on add
	forCollider(collider);

	// Add to set of colliders
	colliders.insert_or_assign(collider, collider->getGameObject()->getTransform()->getWorldMatrix());
}

void CollisionManager::remove(Collider * collider)
{
	colliders.erase(collider);
}

void CollisionManager::forCollider(Collider * collider)
{
	for (std::map<Collider*, DirectX::XMMATRIX>::const_iterator iterator = colliders.begin(), end = colliders.end(); iterator != end; ++iterator) {

		// Retrieve iterator variables
		std::pair<Collider*, DirectX::XMMATRIX> pair = *iterator;
		Collider* other = pair.first;

		// Dont collide with yourself
		if (collider->getGameObject() == other->getGameObject()) continue;

		// Initialize collision values
		float mtvDistance;
		XMVECTOR mtvAxis;

		// Check collision
		if (collider->checkCollision(other, mtvAxis, mtvDistance)) {

			// Generate collision object if a collision took place
			Collision* collision = new Collision(other, mtvAxis, mtvDistance);

			// Handle the collision
			collider->onCollision(collision);

			// Tell the game object to forward the collision to handlers
			collider->getGameObject()->onCollision(collider, collision);

			// Free memory
			delete collision;
		}
	}
}
