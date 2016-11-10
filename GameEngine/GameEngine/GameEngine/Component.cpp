#pragma once

#include "Component.h"

GameObject* Component::getGameObject()
{
	return gameObject;
}

void Component::setGameObject(GameObject * gameObject)
{
	this->gameObject = gameObject;
}
