#pragma once

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "Utility.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "Simplex.h"

BoxCollider::BoxCollider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger, DirectX::XMVECTOR size) : Collider(manager, centerOffset, isTrigger)
{
	this->dimensions = size;
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
	LogInfo("Collision!");
}

// The algorithm used currently expects AABBs
std::set<DirectX::XMVECTOR, VectorCompare>* BoxCollider::checkCollision(Collider * other)
{
	
	// Colliding with oneself makes no sense.
	if (other == this) return nullptr;

	// Get current collider position, including the offset
	DirectX::XMVECTOR currentPos = getCurrentPosition();
	DirectX::XMVECTOR otherPos = DirectX::XMVectorAdd(other->getGameObject()->getTransform()->getWorldPositionVector(), other->getCenterOffset());

	// The GJK algorithm is not being completely implemented and therefore not used for collision checks
	// // Initialize the simplex object
	//auto simplex = Simplex(this, other, DirectX::XMVectorSubtract(otherPos, currentPos));

	//auto result = simplex.check();

	//delete &simplex;

	auto dimensions = getDimensions();
	auto minX = DirectX::XMVectorAdd(currentPos, { -dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
	auto maxX = DirectX::XMVectorAdd(currentPos, { dimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
	auto minY = DirectX::XMVectorAdd(currentPos, { 0.0f, -dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
	auto maxY = DirectX::XMVectorAdd(currentPos, { 0.0f, dimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
	auto minZ = DirectX::XMVectorAdd(currentPos, { 0.0f, 0.0f, -dimensions.m128_f32[2], 0.0f }).m128_f32[2];
	auto maxZ = DirectX::XMVectorAdd(currentPos, { 0.0f, 0.0f, dimensions.m128_f32[2], 0.0f }).m128_f32[2];

	if (instanceof<BoxCollider>(other)) {
		auto otherBoxCollider = (BoxCollider*)other;

		auto otherDimensions = otherBoxCollider->getDimensions();
		auto otherMinX = DirectX::XMVectorAdd(otherPos, { -otherDimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto otherMaxX = DirectX::XMVectorAdd(otherPos, { otherDimensions.m128_f32[0], 0.0f, 0.0f, 0.0f }).m128_f32[0];
		auto otherMinY = DirectX::XMVectorAdd(otherPos, { 0.0f, -otherDimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto otherMaxY = DirectX::XMVectorAdd(otherPos, { 0.0f, otherDimensions.m128_f32[1], 0.0f, 0.0f }).m128_f32[1];
		auto otherMinZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, -otherDimensions.m128_f32[2], 0.0f }).m128_f32[2];
		auto otherMaxZ = DirectX::XMVectorAdd(otherPos, { 0.0f, 0.0f, otherDimensions.m128_f32[2], 0.0f }).m128_f32[2];

		return (otherMinX <= maxX && otherMaxX >= minX) &&
			(otherMinY <= maxY && otherMaxY >= minY) &&
			(otherMinZ <= maxZ && otherMaxZ >= minZ) ? new std::set<DirectX::XMVECTOR, VectorCompare>() : nullptr;
	}
	else if (instanceof<SphereCollider>(other)) {
		auto otherSphereCollider = (SphereCollider*)other;

		auto x = max(minX, otherPos.m128_f32[0], maxX);
		auto y = max(minY, otherPos.m128_f32[1], maxY);
		auto z = max(minZ, otherPos.m128_f32[2], maxZ);

		auto distance = sqrt((x - otherPos.m128_f32[0]) * (x - otherPos.m128_f32[0]) +
			(y - otherPos.m128_f32[1]) * (y - otherPos.m128_f32[1]) +
			(z - otherPos.m128_f32[2]) * (z - otherPos.m128_f32[2]));

		return (distance < otherSphereCollider->getRadius()) ? new std::set<DirectX::XMVECTOR, VectorCompare>() : nullptr;
	}

	return nullptr;
}

DirectX::XMVECTOR BoxCollider::getDimensions()
{
	return this->dimensions;
}
