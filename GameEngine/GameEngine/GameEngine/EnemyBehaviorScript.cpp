#pragma once

#include "EnemyBehaviorScript.h"
#include "GameObject.h"
#include "Transform.h"

EnemyBehaviorScript::EnemyBehaviorScript()
{
	// Do nothing
}

EnemyBehaviorScript::EnemyBehaviorScript(Transform * playerTransform)
{
	this->playerTransform = playerTransform;
}

void EnemyBehaviorScript::update()
{

	// Look towards player
	getGameObject()->getTransform()->localRotation->pitch = -playerTransform->localRotation->pitch;
	getGameObject()->getTransform()->localRotation->yaw = -playerTransform->localRotation->yaw;
	getGameObject()->getTransform()->localRotation->roll = -playerTransform->localRotation->roll;

	// Shoot
	shoot();
}

void EnemyBehaviorScript::setPlayerTransform(Transform * playerTransform)
{
	this->playerTransform = playerTransform;
}
