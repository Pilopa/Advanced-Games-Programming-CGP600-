#pragma once

#include "Shader.h"

#define PixelShaderLanguageVersion "ps_4_0"

class PixelShader : public Shader {

	public:
		ID3D11PixelShader* getD3DPixelShaderInstance();
		ID3D11SamplerState* getSamplerState();
		void setSamplerState(ID3D11SamplerState* samplerState);

		static PixelShader* loadFromFile(LPCWSTR path, D3D11_BUFFER_DESC* constant_buffer_desc = nullptr, D3D11_SAMPLER_DESC* samplerDesc = nullptr);

	private:
		ID3D11PixelShader* d3DPixelShaderInstance = nullptr;
		ID3D11SamplerState* samplerState = nullptr;

};