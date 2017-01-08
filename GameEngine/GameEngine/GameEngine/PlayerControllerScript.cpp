#pragma once

#include <string>
#include <DirectXMath.h>
#include "PlayerControllerScript.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Debug.h"

using namespace DirectX;

void PlayerControllerScript::update() 
{
	// Call base method
	TankBehaviorScript::update();

	// Movement
	if (InputManager::instance()->getKey(KEY_W)) {

		DirectX::XMVECTOR movementVector = getGameObject()->getTransform()->getLocalDirectionalVector();

		movementVector.m128_f32[0] *= moveSpeed * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[1] *= moveSpeed * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[2] *= moveSpeed * (float)GameManager::instance()->getDeltaTime();

		getGameObject()->getTransform()->translate(movementVector);
	}

	if (InputManager::instance()->getKey(KEY_S)) {

		DirectX::XMVECTOR movementVector = getGameObject()->getTransform()->getLocalDirectionalVector();

		movementVector.m128_f32[0] *= -moveSpeed * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[1] *= -moveSpeed * (float)GameManager::instance()->getDeltaTime();
		movementVector.m128_f32[2] *= -moveSpeed * (float)GameManager::instance()->getDeltaTime();

		getGameObject()->getTransform()->translate(movementVector);

	}

	if (InputManager::instance()->getKey(KEY_A)) {
		if (InputManager::instance()->getKey(KEY_S))
			getGameObject()->getTransform()->rotate(AXIS_Y, turnSpeed * (float)GameManager::instance()->getDeltaTime());
		else
			getGameObject()->getTransform()->rotate(AXIS_Y, -turnSpeed * (float)GameManager::instance()->getDeltaTime());
	} 
	
	if (InputManager::instance()->getKey(KEY_D)) {
		if (InputManager::instance()->getKey(KEY_S))
			getGameObject()->getTransform()->rotate(AXIS_Y, -turnSpeed * (float)GameManager::instance()->getDeltaTime());
		else
			getGameObject()->getTransform()->rotate(AXIS_Y, turnSpeed * (float)GameManager::instance()->getDeltaTime());
	}

	if (InputManager::instance()->getKeyDown(SPACE)) {
		shoot();
	}
}
