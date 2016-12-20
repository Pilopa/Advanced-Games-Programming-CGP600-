#pragma once

#include "Collision.h"

Collision::Collision(const Collider * other, std::set<DirectX::XMVECTOR> pointsOfImpact)
{
	this->other = other;
	this->pointsOfImpact = &pointsOfImpact;
}

const Collider * Collision::getOther()
{
	return other;
}

const std::set<DirectX::XMVECTOR>* Collision::getPointsOfImpact()
{
	return pointsOfImpact;
}
