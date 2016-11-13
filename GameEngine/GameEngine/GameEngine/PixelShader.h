#pragma once

#include "Shader.h"

#define PixelShaderLanguageVersion "ps_4_0"

class PixelShader : public Shader {

	public:
		ID3D11PixelShader* getD3DPixelShaderInstance();
		ID3D11SamplerState* getSamplerState();

		static PixelShader* loadFromFile(LPCWSTR path, D3D11_BUFFER_DESC* constant_buffer_desc = NULL);

	private:
		ID3D11PixelShader* d3DPixelShaderInstance = nullptr;
		ID3D11SamplerState* samplerState = nullptr;

};