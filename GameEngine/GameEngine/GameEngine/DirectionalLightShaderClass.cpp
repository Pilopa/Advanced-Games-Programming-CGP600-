#pragma once

#include "DirectionalLightShaderClass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsManager.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"

DirectionalLightShaderClass::DirectionalLightShaderClass()
{
	// Create matrix buffer description
	D3D11_BUFFER_DESC matrix_buffer_desc;
	ZeroMemory(&matrix_buffer_desc, sizeof(matrix_buffer_desc));

	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBuffer); // Must be multiple of 16, calculate from CB struct
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as constant buffer
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.StructureByteStride = 0;

	// Create light buffer description
	D3D11_BUFFER_DESC light_buffer_desc;
	ZeroMemory(&light_buffer_desc, sizeof(light_buffer_desc));

	light_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	light_buffer_desc.ByteWidth = sizeof(LightBuffer); // Must be multiple of 16, calculate from CB struct
	light_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as constant buffer
	light_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	light_buffer_desc.MiscFlags = 0;
	light_buffer_desc.StructureByteStride = 0;

	// Load shaders

	vertexShader = VertexShader::loadFromFile(L"DeferredDirectionalLightVertexShader.hlsl", &matrix_buffer_desc);
	pixelShader = PixelShader::loadFromFile(L"DeferredDirectionalLightPixelShader.hlsl", &light_buffer_desc);
}

void DirectionalLightShaderClass::prepare(GameObject * object)
{
	// Update pixel shader constant buffer
	LightBuffer lightBufferValues;
	lightBufferValues.lightDirection = object->getTransform()->getDirectionalVector();
	lightBufferValues.padding = 0.0F;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GraphicsManager::instance()->getDeviceContext()->Map(pixelShader->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &lightBufferValues, sizeof(LightBuffer));
	GraphicsManager::instance()->getDeviceContext()->Unmap(pixelShader->getConstantBuffer(), 0);

	// Set pixel Shader constant buffer
	ID3D11Buffer* pCB = getPixelShader()->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->PSSetConstantBuffers(0, 1, &pCB);
}
