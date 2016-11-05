#pragma once

#include "Resource.h"
#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"

class Material : public Resource {

	public:
		VertexShader* getVertexShader();
		PixelShader* getPixelShader();

	private:
		Texture* texture = nullptr;
		VertexShader* vertexShader = nullptr;
		PixelShader* pixelShader = nullptr;
		ID3D11Buffer* constantBuffer = nullptr;
};