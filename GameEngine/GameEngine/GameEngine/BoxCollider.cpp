#pragma once

#include <limits>
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "Utility.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "Simplex.h"
#include "Rigidbody.h"

BoxCollider::BoxCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, DirectX::XMVECTOR size) : Collider(manager, centerOffset, isTrigger)
{
	this->dimensions = size;
}

bool BoxCollider::testAxisWithAABB(XMVECTOR axis, float minA, float maxA, float minB, float maxB, XMVECTOR& outMtv, float& outPenetration)
{
	float axisLengthSquared = XMVector3Dot(axis, axis).m128_f32[0];

	// If axis is degenerate then ignore
	if (axisLengthSquared < 1.0e-8f) return true;

	// Calculate possible overlap ranges
	float d0 = (maxB - minA); // left
	float d1 = (maxA - minB); // right

	// Check intervals for intersection
	if (d0 <= 0.0f || d1 <= 0.0f) return false;

	// Check on which side the overlap is
	float overlap = (d0 < d1) ? d0 : -d1;

	// Calculate mtd vector
	XMVECTOR sep = XMVectorMultiply(axis, Vector(overlap / axisLengthSquared));

	// Squared length of mtg vector
	float sepLengthSquared = XMVector3Dot(sep, sep).m128_f32[0];

	// Update mtv if it is the new smallest translation distance
	if (sepLengthSquared < outPenetration) {
		outPenetration = sepLengthSquared;
		outMtv = sep;
	}

	return true;
}

std::vector<DirectX::XMVECTOR>* BoxCollider::getColliderVertices()
{
	// Get current position, including the offset
	DirectX::XMVECTOR currentPos = getCurrentPosition();

	// Build vertex points for comparison
	auto* points = new std::vector<DirectX::XMVECTOR>();
	points->push_back(DirectX::XMVectorAdd(currentPos, { -this->dimensions.m128_f32[0], -this->dimensions.m128_f32[1], this->dimensions.m128_f32[2] })); // back down left
	points->push_back(DirectX::XMVectorAdd(currentPos, { -this->dimensions.m128_f32[0], this->dimensions.m128_f32[1], this->dimensions.m128_f32[2] })); // back up left
	points->push_back(DirectX::XMVectorAdd(currentPos, { this->dimensions.m128_f32[0], this->dimensions.m128_f32[1], this->dimensions.m128_f32[2]})); // back up right
	points->push_back(DirectX::XMVectorAdd(currentPos, { this->dimensions.m128_f32[0], -this->dimensions.m128_f32[1], this->dimensions.m128_f32[2] })); // back down right
	points->push_back(DirectX::XMVectorAdd(currentPos, { this->dimensions.m128_f32[0], -this->dimensions.m128_f32[1], -this->dimensions.m128_f32[2] })); // front down right
	points->push_back(DirectX::XMVectorAdd(currentPos, { -this->dimensions.m128_f32[0], -this->dimensions.m128_f32[1], -this->dimensions.m128_f32[2] })); // front down left
	points->push_back(DirectX::XMVectorAdd(currentPos, { -this->dimensions.m128_f32[0], this->dimensions.m128_f32[1], -this->dimensions.m128_f32[2] })); // front up left
	points->push_back(DirectX::XMVectorAdd(currentPos, { this->dimensions.m128_f32[0], this->dimensions.m128_f32[1], -this->dimensions.m128_f32[2] })); // front up right

	return points;
}

std::vector<DirectX::XMVECTOR>* BoxCollider::getColliderNormals()
{
	// Get vertices
	auto vertices = getColliderVertices();

	// Build normals
	auto* normals = new std::vector<DirectX::XMVECTOR>();
	
	// Go through vertices to find normals
	for (int x = 0; x < vertices->size(); x++) {
		auto y = x + 1 == vertices->size() ? 0 : x + 1;
		auto ownVertexA = (*vertices)[x];
		auto ownVertexB = (*vertices)[y];
		auto normal = DirectX::XMVector3Cross(ownVertexA, ownVertexB);
		normals->push_back(normal);
	}

	// Free memory
	delete vertices;

	// Return normals
	return normals;
}

DirectX::XMVECTOR BoxCollider::getCurrentPosition()
{
	return DirectX::XMVectorAdd(this->getGameObject()->getTransform()->getWorldPositionVector(), this->getCenterOffset());
}

DirectX::XMVECTOR BoxCollider::getFarthestVertexInDirection(DirectX::XMVECTOR direction)
{
	// Retrieve vertices
	auto vertices = getColliderVertices();

	// Predefine temporary work variables
	float largestProjectedMagnitude = 0.0f;
	int farthestIndex = 0;
	
	// Iterate over indices
	for (int i = 0; i < vertices->size(); i++) {

		// Get current index
		auto vertex = vertices->at(i);

		auto projectedVector = DirectX::XMVectorMultiply(DirectX::XMVectorDivide(DirectX::XMVector3Dot(direction, vertex), DirectX::XMVector3Dot(direction, direction)), direction);
		auto projectedMag = DirectX::XMVector3Length(projectedVector).m128_f32[0]; // Each component contains the vectors magnitude

		if (projectedMag > largestProjectedMagnitude) {
			farthestIndex = i;
			largestProjectedMagnitude = projectedMag;
		}
	}

	// Cache result
	auto result = vertices->at(farthestIndex);

	// Free memory
	delete vertices;

	// Return result
	return result;
}

void BoxCollider::onCollision(Collision * collision)
{
	// Do not perform actual collision with trigger colliders
	if (collision->getOther()->isTrigger() || isTrigger()) return;

	// Check collider's game object for rigidbody to determine reaction to collision
	Rigidbody* rigidbody = getGameObject()->getComponent<Rigidbody>();

	if (rigidbody != nullptr) {
		rigidbody->reset(collision->getMinimumTranslationVector()); // Stop rigidbody
	}

	// Push collider back given amount
	this->getGameObject()->getTransform()->translate(XMVectorMultiply(collision->getMinimumTranslationVector(), Vector(collision->getPenetrationAmount())));

}

// The algorithm used currently expects AABBs
bool BoxCollider::checkCollision(Collider * other, XMVECTOR& mtv, float& penetration)
{
	// Colliding with oneself makes no sense.
	if (other == this) return nullptr;

	// Get current collider position, including the offset
	DirectX::XMVECTOR currentPos = getCurrentPosition();
	DirectX::XMVECTOR otherPos = DirectX::XMVectorAdd(other->getGameObject()->getTransform()->getWorldPositionVector(), other->getCenterOffset());

	// Initialize own collider values
	auto dimensions = getDimensions();
	auto minX = DirectX::XMVectorAdd(currentPos, { -dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
	auto maxX = DirectX::XMVectorAdd(currentPos, { dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
	auto minY = DirectX::XMVectorAdd(currentPos, { 0.0f, -dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
	auto maxY = DirectX::XMVectorAdd(currentPos, { 0.0f, dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
	auto minZ = DirectX::XMVectorAdd(currentPos, { 0.0f, 0.0f, -dimensions.m128_f32[2], 0.0f }).m128_f32[2];
	auto maxZ = DirectX::XMVectorAdd(currentPos, { 0.0f, 0.0f, dimensions.m128_f32[2], 0.0f }).m128_f32[2];

	if (instanceof<BoxCollider>(other)) {

		// Initialize collision values
		float mtvDistance = FLT_MAX;
		XMVECTOR mtvAxis;

		// Initialize over collider values
		auto otherBoxCollider = (BoxCollider*)other;
		auto otherDimensions = otherBoxCollider->getDimensions();
		auto otherMinX = DirectX::XMVectorAdd(otherPos, { -otherDimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto otherMaxX = DirectX::XMVectorAdd(otherPos, { otherDimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto otherMinY = DirectX::XMVectorAdd(otherPos, { 0.0f, -otherDimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto otherMaxY = DirectX::XMVectorAdd(otherPos, { 0.0f, otherDimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto otherMinZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, -otherDimensions.m128_f32[2], 0.0f }).m128_f32[2];
		auto otherMaxZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, otherDimensions.m128_f32[2], 0.0f }).m128_f32[2];

		// Test X Axis
		if (!testAxisWithAABB(XVector, minX, maxX, otherMinX, otherMaxX, mtvAxis, mtvDistance)) return false;

		// Test Y Axis
		if (!testAxisWithAABB(YVector, minY, maxY, otherMinY, otherMaxY, mtvAxis, mtvDistance)) return false;

		// Test Z Axis
		if (!testAxisWithAABB(ZVector, minZ, maxZ, otherMinZ, otherMaxZ, mtvAxis, mtvDistance)) return false;

		// Calculate minimum translation vector
		mtv = XMVector3Normalize(mtvAxis);

		// Calculate penetration depth
		penetration = sqrt(mtvDistance) * PENETRATION_INCREMENT;

		// Collision successfully calculated
		return true;

	}

	return false;
	
}

DirectX::XMVECTOR BoxCollider::getDimensions()
{
	return this->dimensions;
}
