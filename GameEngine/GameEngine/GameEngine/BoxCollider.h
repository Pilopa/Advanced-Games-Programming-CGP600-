#pragma once

#include <vector>
#include "Collider.h"

class BoxCollider : public Collider {

	public:
		void onCollision(Collision* collision); // Overrides Collider::onCollision 
		std::set<DirectX::XMVECTOR, VectorCompare>* checkCollision(Collider* other); // Overrides Collider::checkCollision 

		DirectX::XMVECTOR getDimensions();
		BoxCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, DirectX::XMVECTOR size);

	private:
		DirectX::XMVECTOR dimensions = ZERO_VECTOR;

		std::vector<DirectX::XMVECTOR>* getColliderVertices();
		std::vector<DirectX::XMVECTOR>* getColliderNormals();
		DirectX::XMVECTOR getCurrentPosition();

		DirectX::XMVECTOR getFarthestVertexInDirection(DirectX::XMVECTOR direction); // Overrides Collider::getFarthestVertexInDirection
};