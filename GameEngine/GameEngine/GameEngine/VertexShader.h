#pragma once

#include "Shader.h"

#define VertexShaderLanguageVersion "vs_4_0"

class VertexShader : public Shader {

	public:
		ID3D11VertexShader* getD3DVertexShaderInstance();

		static VertexShader* loadFromFile(LPCWSTR path);

	private:
		ID3D11VertexShader* d3DVertexShaderInstance = nullptr;

};