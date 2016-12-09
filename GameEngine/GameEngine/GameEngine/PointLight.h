#pragma once

#include "Light.h"

class PointLight : public Light {

	public:

		virtual void render(); // Overrides Light::render

		PointLight(float color[4], float intensity);

};