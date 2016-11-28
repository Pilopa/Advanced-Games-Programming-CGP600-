#pragma once

#include <array>
#include "Light.h"
#include "LightingManager.h"

void Light::update()
{
	// Do nothing
}

void Light::awake()
{
	LightingManager::instance()->registerLight(this);
}

void Light::shutdown()
{
	LightingManager::instance()->deregisterLight(this);
}

Light::Light(float color[4], float intensity)
{
	this->color[0] = color[0];
	this->color[1] = color[1];
	this->color[2] = color[2];
	this->color[3] = color[3];
	this->intensity = intensity;
}

float * Light::getColor()
{
	return color;
}

float Light::getIntensity()
{
	return intensity;
}

ShaderClass * Light::getShaderClass()
{
	return shaderClass;
}
