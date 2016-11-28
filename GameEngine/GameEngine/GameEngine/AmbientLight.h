#pragma once

#include "Light.h"

class AmbientLight : public Light {

	public:
		virtual void render(); // Overrides Light::render

		AmbientLight(float color[4], float intensity);

};