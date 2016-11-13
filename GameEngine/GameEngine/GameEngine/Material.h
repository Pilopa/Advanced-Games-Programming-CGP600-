#pragma once

#include "Resource.h"
#include "ClassDef.h"

struct MATERIAL_CONSTANT_BUFFER {

};

class Material : public Resource {

	public:
		Material(ShaderClass * shaderClass, Texture* texture);
		ShaderClass* getShaderClass();
		Texture* getTexture();

	private:
		Texture* texture = nullptr;
		ShaderClass* shaderClass = nullptr;
};