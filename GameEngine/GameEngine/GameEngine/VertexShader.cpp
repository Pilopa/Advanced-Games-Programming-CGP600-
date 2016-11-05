#pragma once

#include <d3dcompiler.h>
#include <string>
#include "GraphicsManager.h"
#include "VertexShader.h"
#include "Debug.h"

ID3D11VertexShader * VertexShader::getD3DVertexShaderInstance()
{
	return d3DVertexShaderInstance;
}

VertexShader* VertexShader::loadFromFile(LPCWSTR path)
{
	// Declare temporary variables
	HRESULT result = S_OK;
	ID3DBlob *shaderResource, *error;

	// Declare Result variable
	VertexShader* vertexShader = new VertexShader();

	// Load the vertex shader
	result = D3DCompileFromFile(path, 0, 0, "main", VertexShaderLanguageVersion, 0, 0, &shaderResource, &error);

	if (error != 0) { // Check for shader compilation error
		LogError((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(result)) { // Dont fail if erro is just a warning
			return nullptr;
		}
	}

	// Construct the vertex shader instance
	result = GraphicsManager::instance()->getDevice()->CreateVertexShader(shaderResource->GetBufferPointer(), shaderResource->GetBufferSize(), nullptr, &vertexShader->d3DVertexShaderInstance);

	if (FAILED(result)) {
		LogError("VertexShader instance could not be created!");
		return nullptr;
	}

	return vertexShader;
}
