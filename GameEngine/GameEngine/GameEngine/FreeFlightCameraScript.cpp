#pragma once

#include <DirectXMath.h>
#include "FreeFlightCameraScript.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include "Debug.h"

void FreeFlightCameraScript::update()
{

	if (InputManager::instance()->getKey(KEY_W)) {

		DirectX::XMVECTOR movementVector = GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()->getDirectionalVector();

		movementVector.m128_f32[0] *= 5.0F * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[1] *= 5.0F * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[2] *= 5.0F * (float)GameManager::instance()->getDeltaTime();

		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->translate(movementVector);
	}

	if (InputManager::instance()->getKey(KEY_S)) {

		DirectX::XMVECTOR movementVector = GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()->getDirectionalVector();

		movementVector.m128_f32[0] *= -5.0F * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[1] *= -5.0F * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[2] *= -5.0F * (float)GameManager::instance()->getDeltaTime();

		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->translate(movementVector);

	}

	if (InputManager::instance()->getKey(KEY_A)) {
		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->rotate(AXIS_Y, -45.0F * (float)GameManager::instance()->getDeltaTime());
	}

	if (InputManager::instance()->getKey(KEY_D)) {
		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->rotate(AXIS_Y, 45.0F * (float)GameManager::instance()->getDeltaTime());
	}

	if (InputManager::instance()->getKey(KEY_ARROW_UP)) {
		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->rotate(AXIS_X, 45.0F * (float)GameManager::instance()->getDeltaTime());
	}

	if (InputManager::instance()->getKey(KEY_ARROW_DOWN)) {
		GameManager::instance()->getScene()->getActiveCamera()
			->getGameObject()->getTransform()
			->rotate(AXIS_X, -45.0F * (float)GameManager::instance()->getDeltaTime());
	}

}

void FreeFlightCameraScript::awake()
{
	// Do nothing
}

void FreeFlightCameraScript::shutdown()
{
	// Do nothing
}
