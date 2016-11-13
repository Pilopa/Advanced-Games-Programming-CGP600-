#pragma once

#include "ClassDef.h"

class ShaderClass {

	public:
		VertexShader* getVertexShader();
		PixelShader* getPixelShader();

		// Fill Buffers
		virtual void prepare(GameObject* object) = 0;

	protected:
		VertexShader* vertexShader = nullptr;
		PixelShader* pixelShader = nullptr;
};