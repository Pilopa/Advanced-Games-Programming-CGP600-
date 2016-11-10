#pragma once

#include "Resource.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"

struct MATERIAL_CONSTANT_BUFFER {

};

class Material : public Resource {

	public:
		Material(VertexShader* vertexShader, PixelShader* pixelShader, Texture* texture);
		VertexShader* getVertexShader();
		PixelShader* getPixelShader();

	private:
		Texture* texture = nullptr;
		VertexShader* vertexShader = nullptr;
		PixelShader* pixelShader = nullptr;
};