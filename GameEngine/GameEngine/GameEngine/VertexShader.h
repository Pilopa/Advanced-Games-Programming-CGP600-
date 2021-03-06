#pragma once

#include <DirectXMath.h>
#include "Shader.h"

#define VertexShaderLanguageVersion "vs_4_0"

class VertexShader : public Shader {

	public:
		ID3D11VertexShader* getD3DVertexShaderInstance();
		ID3D11InputLayout* getVertexInputLayout();
		
		static VertexShader* loadFromFile(LPCWSTR path, D3D11_BUFFER_DESC* constant_buffer_desc = nullptr);
		static HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);

	private:
		ID3D11VertexShader* d3DVertexShaderInstance = nullptr;
		ID3D11InputLayout* vertexInputLayout = nullptr;

};