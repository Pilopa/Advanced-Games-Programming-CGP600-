#pragma once

#include "BoxCollider.h"
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

std::set<DirectX::XMVECTOR, VectorCompare>* BoxCollider::checkCollision(Collider * other)
{
	
	// Colliding with oneself makes no sense.
	if (other == this) return nullptr;

	// Get current collider position, including the offset
	DirectX::XMVECTOR currentPos = getCurrentPosition();
	DirectX::XMVECTOR otherPos = DirectX::XMVectorAdd(other->getGameObject()->getTransform()->getWorldPositionVector(), other->getCenterOffset());

	// Setup points of impact collection
	auto pointsOfImpact = new std::set<DirectX::XMVECTOR, VectorCompare>();

	// Initialize origin value
	DirectX::XMVECTOR origin = ZERO_VECTOR;

	// Initialize the simplex object
	auto simplex = Simplex();

	// Initialize direction with difference of both collider's positions
	DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(otherPos, currentPos);

	// Input first point
	auto p = Simplex::Support(this, other, direction);
	simplex.add(p);

	// Negate the direction for the next point
	direction = DirectX::XMVectorNegate(direction);

	// Continue with the iterative algorithm, check if the point past the origin
	while (DirectX::XMVector3GreaterOrEqual(p = DirectX::XMVector3Dot(Simplex::Support(this, other, direction), direction), ZERO_VECTOR)) {

		// Add a new point to the simplex that has not been terminated yet
		simplex.add(p);

		// Check if the simplex contains the origin and updates the direction accordingly
		if (simplex.containsOrigin(&direction)) {

			// If yes, there definitely is a collision:
			// Start EPA
			LogInfo("YES!");
		}
	}

	// Free memory
	delete pointsOfImpact;

	return nullptr;
}

DirectX::XMVECTOR BoxCollider::getDimensions()
{
	return this->dimensions;
}
