#pragma once

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Utility.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"

void SphereCollider::onCollision(Collision* collision)
{
	// Do not perform actual collision with trigger colliders
	if (collision->getOther()->isTrigger() || isTrigger()) return;

	// Calculate positions with offset taken into account
	DirectX::XMVECTOR currentPos = DirectX::XMVectorAdd(getGameObject()->getTransform()->getWorldPositionVector(), this->getCenterOffset());
	DirectX::XMVECTOR otherPos = DirectX::XMVectorAdd(collision->getOther()->getGameObject()->getTransform()->getWorldPositionVector(), collision->getOther()->getCenterOffset());

	// Go through all collision points and move sphere back accordingly
	for (std::set<DirectX::XMVECTOR, VectorCompare>::const_iterator iterator = collision->getPointsOfImpact()->begin(), end = collision->getPointsOfImpact()->end(); iterator != end; ++iterator) {
		auto pointOfImpact = ((DirectX::XMVECTOR)*iterator);

		// Calculate angle between this collider and the point of impact
		auto direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(currentPos, otherPos));

		// Calculate offset
		auto offset = DirectX::XMVectorMultiply(direction, { radius , radius , radius , 1.0f });

		// Calculate new position
		auto newPosition = DirectX::XMVectorAdd(pointOfImpact, offset);
	
		// Finally move sphere back
		this->getGameObject()->getTransform()->localPosition->x = newPosition.m128_f32[0];
		this->getGameObject()->getTransform()->localPosition->y = newPosition.m128_f32[1];
		this->getGameObject()->getTransform()->localPosition->z = newPosition.m128_f32[2];
	}
}

std::set<DirectX::XMVECTOR, VectorCompare>* SphereCollider::checkCollision(Collider * other)
{
	// Colliding with oneself makes no sense.
	if (other == this) return nullptr;

	DirectX::XMVECTOR currentPos = DirectX::XMVectorAdd(getGameObject()->getTransform()->getWorldPositionVector(), this->getCenterOffset());
	DirectX::XMVECTOR otherPos = DirectX::XMVectorAdd(other->getGameObject()->getTransform()->getWorldPositionVector(), other->getCenterOffset());

	if (instanceof<SphereCollider>(other))
	{
		SphereCollider* otherSphereCollider = (SphereCollider*) other;
		
		// Calculate squared distance
		float squareDistance = (
			(currentPos.m128_f32[0] - otherPos.m128_f32[0]) * (currentPos.m128_f32[0] - otherPos.m128_f32[0]) +
			(currentPos.m128_f32[1] - otherPos.m128_f32[1]) * (currentPos.m128_f32[1] - otherPos.m128_f32[1]) +
			(currentPos.m128_f32[2] - otherPos.m128_f32[2]) * (currentPos.m128_f32[2] - otherPos.m128_f32[2])
		);
	
		// Squared radii
		float radiiSumSquared = this->radius + otherSphereCollider->radius;
		radiiSumSquared *= radiiSumSquared;

		// Check collision
		if (squareDistance <= radiiSumSquared) {

			// Calculate angle between this collider and the point of impact
			auto direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(currentPos, otherPos));

			// Calculate offset
			auto offset = DirectX::XMVectorMultiply(direction, { otherSphereCollider->radius , otherSphereCollider->radius , otherSphereCollider->radius , 1.0f });

			// Calculate new position
			auto pointOfImpact = DirectX::XMVectorAdd(otherSphereCollider->getGameObject()->getTransform()->getWorldPositionVector(), offset);

			// Build result
			auto result = new std::set<DirectX::XMVECTOR, VectorCompare>();
			result->insert(pointOfImpact);

			// Generate point of impact
			return result;
		}
	} else if (instanceof<BoxCollider>(other)) {
		auto otherBoxCollider = (BoxCollider*)other;

		auto dimensions = otherBoxCollider->getDimensions();
		auto minX = DirectX::XMVectorAdd(otherPos, { -dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto maxX = DirectX::XMVectorAdd(otherPos, { dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto minY = DirectX::XMVectorAdd(otherPos, { 0.0f, -dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto maxY = DirectX::XMVectorAdd(otherPos, { 0.0f, dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto minZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, -dimensions.m128_f32[2], 0.0f }).m128_f32[2];
		auto maxZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, dimensions.m128_f32[2], 0.0f }).m128_f32[2];

		auto x = max(minX, currentPos.m128_f32[0], maxX);
		auto y = max(minY, currentPos.m128_f32[1], maxY);
		auto z = max(minZ, currentPos.m128_f32[2], maxZ);

		auto distance = sqrt((x - currentPos.m128_f32[0]) * (x - currentPos.m128_f32[0]) +
			(y - currentPos.m128_f32[1]) * (y - currentPos.m128_f32[1]) +
			(z - currentPos.m128_f32[2]) * (z - currentPos.m128_f32[2]));

		return (distance < getRadius()) ? new std::set<DirectX::XMVECTOR, VectorCompare>() : nullptr;
	}

	return nullptr;
}

float SphereCollider::getRadius()
{
	return radius;
}

SphereCollider::SphereCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, float radius) : Collider(manager, centerOffset, isTrigger)
{
	this->radius = radius;
}