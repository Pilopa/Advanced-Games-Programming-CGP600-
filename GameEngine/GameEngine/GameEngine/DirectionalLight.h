#pragma once

#include "Light.h"

class DirectionalLight : public Light {

	public:
		virtual void render(); // Overrides Light::render

		DirectionalLight(float color[4], float intensity);

};