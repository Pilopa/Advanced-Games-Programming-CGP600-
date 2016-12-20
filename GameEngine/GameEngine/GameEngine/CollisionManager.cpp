#pragma once

#include "CollisionManager.h"
#include "Collision.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Utility.h"

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

void CollisionManager::registerCollider(Collider * collider)
{
	// Do initial collision check on add
	forCollider(collider);

	// Add to set of colliders
	colliders.insert_or_assign(collider, collider->getGameObject()->getTransform()->getWorldMatrix());
}

void CollisionManager::deregisterCollider(Collider * collider)
{
	colliders.erase(collider);
}

void CollisionManager::forCollider(Collider * collider)
{
	for (std::map<Collider*, DirectX::XMMATRIX>::const_iterator iterator = colliders.begin(), end = colliders.end(); iterator != end; ++iterator) {
		std::pair<Collider*, DirectX::XMMATRIX> pair = *iterator;
		Collider* other = pair.first;
		std::set<DirectX::XMVECTOR>* pointsOfImpact = collider->checkCollision(other);
		if (pointsOfImpact != nullptr) {
			collider->onCollision(new Collision(other, *pointsOfImpact));
		}
	}
}
