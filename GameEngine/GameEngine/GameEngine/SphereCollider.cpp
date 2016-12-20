#pragma once

#include "SphereCollider.h"
#include "Collision.h"
#include "Utility.h"

void SphereCollider::onCollision(const Collision* collision)
{

	delete collision;
}

std::set<DirectX::XMVECTOR>* SphereCollider::checkCollision(Collider * other)
{
	if (instanceof<SphereCollider>(other))
	{
		SphereCollider* sphereCollider = (SphereCollider*) other;

	}

	return NULL;
}

float SphereCollider::getRadius()
{
	return radius;
}

SphereCollider::SphereCollider(float radius)
{
	this->radius = radius;
}
