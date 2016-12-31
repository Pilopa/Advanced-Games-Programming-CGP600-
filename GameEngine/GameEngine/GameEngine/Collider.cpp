#pragma once

#include "Collider.h"
#include "Collision.h"

void Collider::awake() {
	manager->add(this);
}

void Collider::shutdown()
{
	manager->remove(this);
}

DirectX::XMVECTOR Collider::getCenterOffset()
{
	return this->centerOffset;
}

bool Collider::isTrigger()
{
	return trigger;
}

DirectX::XMVECTOR Collider::getFarthestVertexInDirection(DirectX::XMVECTOR direction)
{
	return DirectX::XMVECTOR();
}

void Collider::update() {
	// Do nothing
}


Collider::Collider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger)
{
	this->manager = manager;
	this->centerOffset = centerOffset;
	this->trigger = isTrigger;
}