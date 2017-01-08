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

	// Push collider back
	this->getGameObject()->getTransform()->translate(XMVectorMultiply(collision->getMinimumTranslationVector(), Vector(collision->getPenetrationAmount())));
}

bool SphereCollider::checkCollision(Collider* other, XMVECTOR& outMtv, float& outPenetration)
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
		if (squareDistance > radiiSumSquared) return false;

		// Calculate angle between this collider and the point of impact
		outMtv = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(currentPos, otherPos));

		// Calculate offset
		outPenetration = (this->radius + otherSphereCollider->radius) * PENETRATION_INCREMENT;

		// Collision successfully calculated
		return true;

	} else if (instanceof<BoxCollider>(other)) {
		auto otherBoxCollider = (BoxCollider*)other;

		auto dimensions = otherBoxCollider->getDimensions();
		auto minX = DirectX::XMVectorAdd(otherPos, { -dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto maxX = DirectX::XMVectorAdd(otherPos, { dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto minY = DirectX::XMVectorAdd(otherPos, { 0.0f, -dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto maxY = DirectX::XMVectorAdd(otherPos, { 0.0f, dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto minZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, -dimensions.m128_f32[2], 0.0f }).m128_f32[2];
		auto maxZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, dimensions.m128_f32[2], 0.0f }).m128_f32[2];

		auto x = max(minX, min(currentPos.m128_f32[0], maxX));
		auto y = max(minY, min(currentPos.m128_f32[1], maxY));
		auto z = max(minZ, min(currentPos.m128_f32[2], maxZ));

		auto distance = sqrt((x - currentPos.m128_f32[0]) * (x - currentPos.m128_f32[0]) +
			(y - currentPos.m128_f32[1]) * (y - currentPos.m128_f32[1]) +
			(z - currentPos.m128_f32[2]) * (z - currentPos.m128_f32[2]));

		if (distance < getRadius()) {
			// TODO: Currently no mtv and penetration depth calculated, will cause visual artifacts
			return true;
		}
	}
}

float SphereCollider::getRadius()
{
	return radius;
}

SphereCollider::SphereCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, float radius) : Collider(manager, centerOffset, isTrigger)
{
	this->radius = radius;
}
