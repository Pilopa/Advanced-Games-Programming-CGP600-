#pragma once

#include "Collider.h"

class SphereCollider : public Collider {

	public:
		virtual void onCollision(const Collision* collision); // Overrides Collider::onCollision 
		virtual std::set<DirectX::XMVECTOR>* checkCollision(Collider* other); // Overrides Collider::checkCollision 

		float getRadius();
		SphereCollider(float radius);

	private:
		float radius = 1.0f;
};