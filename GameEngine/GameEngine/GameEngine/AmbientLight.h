#pragma once

#include "Light.h"

class AmbientLight : public Light {

	virtual void render(); // Overrides Light::render

};