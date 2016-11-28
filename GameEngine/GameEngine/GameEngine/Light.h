#pragma once

#include "ClassDef.h"
#include "Component.h"

class Light : public Component {

	public:
		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void shutdown(); // Overrides Component::shutdown
		virtual void render() = 0;

		Light(float color[4], float intensity);

		float* getColor();
		float getIntensity();
		ShaderClass* getShaderClass();

	protected:
		float color[4] = {0.0F, 0.0F, 0.0F, 1.0F};
		float intensity = 1.0F;
		ShaderClass* shaderClass;
};