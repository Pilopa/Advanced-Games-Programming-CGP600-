#pragma once

#include "Rigidbody.h"
#include "GameObject.h"
#include "Transform.h"
#include "GameManager.h"

void Rigidbody::update()
{
	tick();
}

void Rigidbody::awake()
{
	// Do nothing
}

void Rigidbody::shutdown()
{
	// Do nothing
}

XMVECTOR Rigidbody::getVelocity()
{
	return velocity;
}

XMVECTOR Rigidbody::getTimestepVelocity()
{
	return XMVectorMultiply(velocity, Vector((float)GameManager::instance()->getDeltaTime()));
}

void Rigidbody::addForce(XMVECTOR force)
{
	velocity = XMVectorAdd(velocity, force);
}

void Rigidbody::reset()
{
	velocity = ZERO_VECTOR;
}

void Rigidbody::reset(XMVECTOR axis)
{
	if (axis.m128_f32[0] > 0.0f) velocity.m128_f32[0] = 0.0f;
	if (axis.m128_f32[1] > 0.0f) velocity.m128_f32[1] = 0.0f;
	if (axis.m128_f32[2] > 0.0f) velocity.m128_f32[2] = 0.0f;
}

void Rigidbody::tick()
{
	// Store delta time
	XMVECTOR deltaTime = Vector((float)GameManager::instance()->getDeltaTime());

	// Add gravity 
	addForce(XMVectorMultiply(GRAVITY, deltaTime));
	
	// Calculate the velocity for this step
	auto deltaVelocity = XMVectorMultiply(velocity, deltaTime);

	// Change objects position according to velocity
	getGameObject()->getTransform()->translate(deltaVelocity);
}
