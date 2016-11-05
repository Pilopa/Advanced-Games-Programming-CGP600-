#pragma once

#include "LightingManager.h"

void LightingManager::registerLight(Light * light)
{
	lights.insert(light);
}

void LightingManager::deregisterLight(Light * light)
{
	lights.erase(light);
}
