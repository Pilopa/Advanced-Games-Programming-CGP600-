#pragma once

#include "Resource.h"
#include "ClassDef.h"

class Material : public Resource {

	public:
		Material(ShaderClass * shaderClass, Texture* texture);
		ShaderClass* getShaderClass();
		Texture* getTexture();

	private:
		Texture* texture = nullptr;
		ShaderClass* shaderClass = nullptr;
};