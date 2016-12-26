#pragma once

#include "Collision.h"

Collision::Collision(Collider * other, std::set<DirectX::XMVECTOR, VectorCompare>* pointsOfImpact)
{
	this->other = other;
	this->pointsOfImpact = pointsOfImpact;
}

Collider * Collision::getOther()
{
	return other;
}

std::set<DirectX::XMVECTOR, VectorCompare>* Collision::getPointsOfImpact()
{
	return pointsOfImpact;
}
