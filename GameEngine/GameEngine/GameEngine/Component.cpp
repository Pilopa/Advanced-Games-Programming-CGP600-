#pragma once

#include "Component.h"

GameObject* Component::getGameObject() const
{
	return gameObject;
}

void Component::setGameObject(GameObject * gameObject)
{
	this->gameObject = gameObject;
}
