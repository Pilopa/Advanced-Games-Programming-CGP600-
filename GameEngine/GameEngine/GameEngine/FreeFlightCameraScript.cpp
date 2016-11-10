#pragma once

#include "FreeFlightCameraScript.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Debug.h"

void FreeFlightCameraScript::update()
{
	if (InputManager::instance()->getKey(KEY_W)) {
		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->translate({ 0.0F, 0.0F, 1.0F * (float) GameManager::instance()->getDeltaTime() });
	}
	else if (InputManager::instance()->getKey(KEY_S)) {
		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->translate({ 0.0F, 0.0F, -1.0F * (float)GameManager::instance()->getDeltaTime() });
	}
}

void FreeFlightCameraScript::awake()
{
}