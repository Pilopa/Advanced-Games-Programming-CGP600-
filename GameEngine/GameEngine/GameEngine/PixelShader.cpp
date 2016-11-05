#include "PixelShader.h"

#include <d3dcompiler.h>
#include <string>
#include "GraphicsManager.h"
#include "VertexShader.h"
#include "Debug.h"

ID3D11PixelShader * PixelShader::getD3DPixelShaderInstance()
{
	return d3DPixelShaderInstance;
}

PixelShader* PixelShader::loadFromFile(LPCWSTR path)
{
	// Declare temporary variables
	HRESULT result = S_OK;
	ID3DBlob *shaderResource, *error;

	// Declare Result variable
	PixelShader* pixelShader = new PixelShader();

	// Load the pixel shader
	result = D3DCompileFromFile(path, 0, 0, "main", PixelShaderLanguageVersion, 0, 0, &shaderResource, &error);

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

	return pixelShader;
}
