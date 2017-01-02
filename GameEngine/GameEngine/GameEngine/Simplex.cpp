#pragma once

#include <algorithm>
#include "Simplex.h"
#include "Collider.h"
#include "Debug.h"

DirectX::XMVECTOR Simplex::Support(Collider * a, Collider * b, DirectX::XMVECTOR direction)
{
	return DirectX::XMVectorSubtract(a->getFarthestVertexInDirection(direction), b->getFarthestVertexInDirection(DirectX::XMVectorNegate(direction)));
}

Edge Simplex::getClosestEdge()
{
	Edge closest = Edge();

	// Iterate over all points of the simplex
	for (int i = 0; i < points.size(); i++) {

		// Compute following index
		int j = i + 1 == points.size() ? 0 : i + 1;

		// Get current vectors
		DirectX::XMVECTOR a = points[i];
		DirectX::XMVECTOR b = points[j];

		// Create edge vector
		auto e = DirectX::XMVectorSubtract(b, a);

		// Vector from origin to a (a - zero)
		auto oa = a;

		// vector from edge towards origin
		auto n = DirectX::XMVector3Normalize(TripleProduct(e, oa, e));

		// Distance from origin to edge
		auto d = DirectX::XMVector3Dot(n, a);

		// Check if distance is closest
		if (DirectX::XMVector3Less(d, closest.distance)) {
			closest.distance = d; // Each value of the distance-vector d is the same
			closest.normal = n;
			closest.index = j;
		}
	}

	return closest;
}

void Simplex::add(DirectX::XMVECTOR vector)
{
	points.push_back(vector);
}

bool Simplex::containsOrigin(DirectX::XMVECTOR* outputDirection)
{
	// Get last point of simplex
	auto a = last();

	// Compute ao
	auto ao = DirectX::XMVectorNegate(a);

	// Compute ab edge
	auto ab = DirectX::XMVectorSubtract(B(), A());

	// Check if the simplex is yet a triangle
	if (points.size() == 3) {

		// Compute ac edge
		auto ac = DirectX::XMVectorSubtract(C(), A());

		// Compute normals
		auto abPerp = TripleProduct(ac, ab, ab);
		auto acPerp = TripleProduct(ab, ac, ac);

		// Is origin in R4
		if (DirectX::XMVector3Greater(DirectX::XMVector3Dot(abPerp, ao), ZERO_VECTOR)) {
			// Remove point c
			remove(C());

			// Update the direction for the next iteration step
			*outputDirection = abPerp;
		}
		else {

			// Is origin in R3
			if (DirectX::XMVector3Greater(DirectX::XMVector3Dot(acPerp, ao), ZERO_VECTOR)) {
				// Remove point b
				remove(B());

				// Update the direction for the next iteration step
				*outputDirection = acPerp;
			}
			else {

				// Otherwise we know its in R5
				return true;

			}
		}
	}
	else { // then its the line segment case
		
		// Calculate AB
		auto abPerp = TripleProduct(ab, ao, ab);

		// Update the direction for the next iteration step
		*outputDirection = abPerp;
		
	}

	return false;
}

DirectX::XMVECTOR Simplex::last()
{
	return points.back();
}

DirectX::XMVECTOR Simplex::A()
{
	return points[0];
}

DirectX::XMVECTOR Simplex::B()
{
	return points[1];
}

DirectX::XMVECTOR Simplex::C()
{
	return points[2];
}

void Simplex::remove(DirectX::XMVECTOR vector)
{
	points.erase(std::remove_if(points.begin(), points.end(), VectorEquals(vector)), points.end());
}

Simplex::Simplex(Collider * a, Collider * b, DirectX::XMVECTOR initialDirection)
{
	cA = a;
	cB = b;
	direction = initialDirection;
}

std::set<DirectX::XMVECTOR, VectorCompare>* Simplex::check()
{
	// Setup points of impact collection
	auto pointsOfImpact = new std::set<DirectX::XMVECTOR, VectorCompare>();

	// Initialize origin value
	DirectX::XMVECTOR origin = ZERO_VECTOR;

	// Input first point
	auto p = Simplex::Support(cA, cB, direction);
	add(p);

	// Negate the direction for the next point
	direction = DirectX::XMVectorNegate(direction);

	// Continue with the iterative algorithm, check if the point past the origin
	while (DirectX::XMVector3GreaterOrEqual(p = DirectX::XMVector3Dot(Simplex::Support(cA, cB, direction), direction), ZERO_VECTOR)) {

		// Add a new point to the simplex that has not been terminated yet
		add(p);

		// Check if the simplex contains the origin and updates the direction accordingly
		if (containsOrigin(&direction)) {

			// If yes, there definitely is a collision:
			// Start EPA
			LogInfo("YES!");
		}
	}

	// Free memory
	delete pointsOfImpact;

	return nullptr;
}
