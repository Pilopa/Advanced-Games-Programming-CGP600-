#pragma once

#include <stdint.h>
#include <D3DCompiler.h>
#include <string>
#include <vector>
#include <d3d11.h>
#include <fstream>
#include "Vertex.h"
#include "GraphicsManager.h"
#include "VertexShader.h"
#include "Debug.h"

ID3D11VertexShader * VertexShader::getD3DVertexShaderInstance()
{
	return d3DVertexShaderInstance;
}

ID3D11InputLayout * VertexShader::getVertexInputLayout()
{
	return vertexInputLayout;
}

VertexShader* VertexShader::loadFromFile(LPCWSTR path, D3D11_BUFFER_DESC* constant_buffer_desc)
{
	// Declare temporary variables
	HRESULT result = S_OK;
	ID3DBlob *shaderResource, *error;

	// Declare Result variable
	VertexShader* vertexShader = new VertexShader();

	// Load the vertex shader
	result = D3DCompileFromFile(path, 0, 0, "main", VertexShaderLanguageVersion, D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &shaderResource, &error);

	if (error != 0) { // Check for shader compilation error
		LogError((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(result)) { // Dont fail if erro is just a warning
			LogError("VertexShader could not be compiled!");
			return nullptr;
		}
	}

	// Construct the vertex shader instance
	result = GraphicsManager::instance()->getDevice()->CreateVertexShader(shaderResource->GetBufferPointer(), shaderResource->GetBufferSize(), nullptr, &vertexShader->d3DVertexShaderInstance);

	if (FAILED(result)) {
		LogError("VertexShader instance could not be created!");
		return nullptr;
	}

	// Create input layout

	result = CreateInputLayoutDescFromVertexShaderSignature(shaderResource, GraphicsManager::instance()->getDevice(), &vertexShader->vertexInputLayout);
	//result = GraphicsManager::instance()->getDevice()->CreateInputLayout(VERTEX_INPUT_DESCRIPTION, 3, shaderResource->GetBufferPointer(), shaderResource->GetBufferSize(), &vertexShader->vertexInputLayout);
	
	if (FAILED(result)) {
		LogError("VertexShader input layout could not be generated!");
		return nullptr;
	}

	if (vertexShader->vertexInputLayout == nullptr)
		LogError("VertexShader input layout is null!");

	// Create constant buffer

	if (constant_buffer_desc != NULL) {

		result = GraphicsManager::instance()->getDevice()->CreateBuffer(constant_buffer_desc, NULL, &vertexShader->constantBuffer);

		if (FAILED(result)) {
			LogError("VertexShader constant buffer could not be generated!");
			return nullptr;
		}

	}

	// Release shader resource
	shaderResource->Release();

	return vertexShader;
}

// Source: https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
HRESULT VertexShader::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (uint32_t i = 0; i< shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}