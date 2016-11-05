#pragma once

#include "Shader.h"

#define PixelShaderLanguageVersion "ps_4_0"

class PixelShader : public Shader {

	public:
		ID3D11PixelShader* getD3DPixelShaderInstance();

		static PixelShader* loadFromFile(LPCWSTR path);

	private:
		ID3D11PixelShader* d3DPixelShaderInstance = nullptr;

};