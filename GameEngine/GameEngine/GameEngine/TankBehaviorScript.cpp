#pragma once

#include <DirectXMath.h>
#include "TankBehaviorScript.h"
#include "Utility.h"
#include "GameObject.h"
#include "Transform.h"
#include "ProjectilePrefab.h"
#include "GameManager.h"
#include "Scene.h"

using namespace DirectX;

void TankBehaviorScript::update()
{
	reloadTimer -= GameManager::instance()->getDeltaTime();
}

void TankBehaviorScript::shoot()
{
	if (reloadTimer <= 0.0f) {
		XMVECTOR offset = Vector(2.0f);
		XMVECTOR forward = getGameObject()->getTransform()->getWorldDirectionalVector();

		auto projectileObject = ProjectilePrefab::instance()->instantiate(XMVectorAdd(getGameObject()->getTransform()->getWorldPositionVector(), XMVectorMultiply(forward, offset)));
		*projectileObject->getTransform()->localRotation = *getGameObject()->getTransform()->localRotation;
		
		getGameObject()->getScene()->createGameObject(projectileObject);
		projectileObject->awake();

		reloadTimer = DEFAULT_RELOAD_TIME;
	}
}
