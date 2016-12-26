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

void Collider::update() {
	// Do nothing
}


Collider::Collider(Manager<Collider>* manager)
{
	this->manager = manager;
}