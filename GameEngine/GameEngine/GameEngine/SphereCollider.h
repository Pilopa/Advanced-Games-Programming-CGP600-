#pragma once

#include "Collider.h"

class SphereCollider : public Collider {

	public:
		void onCollision(Collision* collision); // Overrides Collider::onCollision 
		std::set<DirectX::XMVECTOR, VectorCompare>* checkCollision(Collider* other); // Overrides Collider::checkCollision 

		float getRadius();
		SphereCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, float radius);

	private:
		float radius = 1.0f;
};