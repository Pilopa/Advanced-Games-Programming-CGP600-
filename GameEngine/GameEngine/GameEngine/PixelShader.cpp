#pragma once

#include <d3dcompiler.h>
#include <string>
#include "GraphicsManager.h"
#include "VertexShader.h"
#include "Debug.h"
#include "PixelShader.h"

ID3D11PixelShader * PixelShader::getD3DPixelShaderInstance()
{
	return d3DPixelShaderInstance;
}

ID3D11SamplerState * PixelShader::getSamplerState()
{
	return samplerState;
}

PixelShader* PixelShader::loadFromFile(LPCWSTR path, D3D11_BUFFER_DESC* constant_buffer_desc)
{
	// Declare temporary variables
	HRESULT result = S_OK;
	ID3DBlob *shaderResource, *error;

	// Declare Result variable
	PixelShader* pixelShader = new PixelShader();

	// Load the pixel shader
	result = D3DCompileFromFile(path, 0, 0, "main", PixelShaderLanguageVersion, D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &shaderResource, &error);

	if (error != 0) { // Check for shader compilation error
		LogError((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(result)) { // Dont fail if erro is just a warning
			return nullptr;
		}
	}

	// Construct the pixel shader instance
	result = GraphicsManager::instance()->getDevice()->CreatePixelShader(shaderResource->GetBufferPointer(), shaderResource->GetBufferSize(), nullptr, &pixelShader->d3DPixelShaderInstance);

	if (FAILED(result)) {
		LogError("PixelShader instance could not be created!");
		return nullptr;
	}

	// Create constant buffer

	if (constant_buffer_desc != nullptr) {

		result = GraphicsManager::instance()->getDevice()->CreateBuffer(constant_buffer_desc, NULL, &pixelShader->constantBuffer);

		if (FAILED(result)) {
			LogError("VertexShader constant buffer could not be generated!");
			return nullptr;
		}

	}

	// Release shader resource
	shaderResource->Release();

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = GraphicsManager::instance()->getDevice()->CreateSamplerState(&samplerDesc, &pixelShader->samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return pixelShader;
}
