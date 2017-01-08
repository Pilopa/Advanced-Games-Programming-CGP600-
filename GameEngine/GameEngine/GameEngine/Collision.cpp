#pragma once

#include "Collision.h"

Collision::Collision(Collider* other, XMVECTOR minimumTranslationVector, float penetrationAmount)
{
	this->other = other;
	this->minimumTranslationVector = minimumTranslationVector;
	this->penetrationAmount = penetrationAmount;
}

Collider * Collision::getOther()
{
	return other;
}

XMVECTOR Collision::getMinimumTranslationVector()
{
	return minimumTranslationVector;
}

float Collision::getPenetrationAmount()
{
	return penetrationAmount;
}
