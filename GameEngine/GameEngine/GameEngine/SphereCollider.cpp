#pragma once

#include "SphereCollider.h"
#include "Collision.h"
#include "Utility.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"

void SphereCollider::onCollision(Collision* collision)
{
	LogInfo("Collision!");

	// Go through all collision points and move sphere back accordingly
	for (std::set<DirectX::XMVECTOR, VectorCompare>::const_iterator iterator = collision->getPointsOfImpact()->begin(), end = collision->getPointsOfImpact()->end(); iterator != end; ++iterator) {
		auto pointOfImpact = ((DirectX::XMVECTOR)*iterator);

		// Calculate angle between this collider and the point of impact
		DirectX::XMVECTOR angle = DirectX::XMVector3AngleBetweenVectors(this->getGameObject()->getTransform()->getWorldPositionVector(), pointOfImpact);

		// Calculate new position
		auto newPosition = DirectX::XMVectorAdd(pointOfImpact, DirectX::XMVectorMultiply(angle, {radius, 0.0f, 0.0f, 0.0f}));
	
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

	DirectX::XMVECTOR currentPos = DirectX::XMVectorAdd(getGameObject()->getTransform()->getWorldPositionVector(), this->centerOffset);

	if (instanceof<SphereCollider>(other))
	{
		SphereCollider* otherSphereCollider = (SphereCollider*) other;
		DirectX::XMVECTOR otherPos = DirectX::XMVectorAdd(otherSphereCollider->getGameObject()->getTransform()->getWorldPositionVector(), otherSphereCollider->centerOffset);
		
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
			DirectX::XMVECTOR angle = DirectX::XMVector3AngleBetweenVectors(this->getGameObject()->getTransform()->getWorldPositionVector(), otherSphereCollider->getGameObject()->getTransform()->getWorldPositionVector());

			// Calculate new position
			auto pointOfImpact = DirectX::XMVectorAdd(otherSphereCollider->getGameObject()->getTransform()->getWorldPositionVector(), DirectX::XMVectorMultiply(angle, { otherSphereCollider->radius, 0.0f, 0.0f, 0.0f }));

			// Build result
			auto result = new std::set<DirectX::XMVECTOR, VectorCompare>();
			result->insert(pointOfImpact);

			// Generate point of impact
			return result;
		}
	}

	return nullptr;
}

float SphereCollider::getRadius()
{
	return radius;
}

SphereCollider::SphereCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, float radius) : Collider(manager)
{
	this->radius = radius;
	this->centerOffset = centerOffset;
}
