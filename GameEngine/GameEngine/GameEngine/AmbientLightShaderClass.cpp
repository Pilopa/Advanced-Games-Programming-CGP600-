#pragma once

#include "AmbientLightShaderClass.h"
#include "AmbientLight.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsManager.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"

AmbientLightShaderClass::AmbientLightShaderClass()
{
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

	vertexShader = VertexShader::loadFromFile(L"Deferred2DVertexShader.hlsl", nullptr);
	pixelShader = PixelShader::loadFromFile(L"DeferredAmbientLightPixelShader.hlsl", &light_buffer_desc);
}

void AmbientLightShaderClass::prepare(GameObject * object)
{
	AmbientLight* lightComponent = object->getComponent<AmbientLight>();
	if (lightComponent == nullptr)
		throw std::exception("The game object that has been given to the light shader to parse has no appropriate light component attached to it");

	// Update pixel shader constant buffer
	LightBuffer lightBufferValues;
	lightBufferValues.lightIntensity = lightComponent->getIntensity();
	lightBufferValues.lightColor = {
		lightComponent->getColor()[0],
		lightComponent->getColor()[1],
		lightComponent->getColor()[2],
		lightComponent->getColor()[3]
	};

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GraphicsManager::instance()->getDeviceContext()->Map(pixelShader->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &lightBufferValues, sizeof(LightBuffer));
	GraphicsManager::instance()->getDeviceContext()->Unmap(pixelShader->getConstantBuffer(), 0);

	// Set pixel Shader constant buffer
	ID3D11Buffer* pCB = pixelShader->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->PSSetConstantBuffers(0, 1, &pCB);
}

void AmbientLightShaderClass::prepare(AmbientLight * ambientLight)
{
	// Update pixel shader constant buffer
	LightBuffer lightBufferValues;
	lightBufferValues.lightIntensity = ambientLight->getIntensity();
	lightBufferValues.lightColor = {
		ambientLight->getColor()[0],
		ambientLight->getColor()[1],
		ambientLight->getColor()[2],
		ambientLight->getColor()[3]
	};

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GraphicsManager::instance()->getDeviceContext()->Map(pixelShader->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &lightBufferValues, sizeof(LightBuffer));
	GraphicsManager::instance()->getDeviceContext()->Unmap(pixelShader->getConstantBuffer(), 0);

	// Set pixel Shader constant buffer
	ID3D11Buffer* pCB = pixelShader->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->PSSetConstantBuffers(0, 1, &pCB);
}
