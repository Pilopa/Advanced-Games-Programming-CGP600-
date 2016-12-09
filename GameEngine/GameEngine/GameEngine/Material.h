#pragma once

#include "Resource.h"
#include "ClassDef.h"

class Material : public Resource {

	public:
		Material(ShaderClass * shaderClass, Texture* texture, float shininess);
		ShaderClass* getShaderClass();
		Texture* getTexture();
		float getShininess();

	private:
		Texture* texture = nullptr;
		ShaderClass* shaderClass = nullptr;
		float shininess = 0.0F;
};