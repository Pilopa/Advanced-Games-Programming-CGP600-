#pragma once

#include <vector>
#include "Collider.h"

class BoxCollider : public Collider {

	public:
		void onCollision(Collision* collision); // Overrides Collider::onCollision 
		bool checkCollision(Collider* other, XMVECTOR& outMtv, float& outPenetration); // Overrides Collider::checkCollision

		DirectX::XMVECTOR getDimensions();
		BoxCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, DirectX::XMVECTOR size);

	private:
		// Uses Separating Axis Theorem (SAT)
		bool testAxisWithAABB(XMVECTOR axis, float minA, float maxA, float minB, float maxB, XMVECTOR& outMtv, float& outPenetration);

		DirectX::XMVECTOR dimensions = ZERO_VECTOR;

		std::vector<DirectX::XMVECTOR>* getColliderVertices();
		std::vector<DirectX::XMVECTOR>* getColliderNormals();
		DirectX::XMVECTOR getCurrentPosition();

		DirectX::XMVECTOR getFarthestVertexInDirection(DirectX::XMVECTOR direction); // Overrides Collider::getFarthestVertexInDirection
};