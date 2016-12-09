#pragma once

#include "DirectionalLightShaderClass.h"
#include "DirectionalLight.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsManager.h"
#include "ApplicationManager.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"

DirectionalLightShaderClass::DirectionalLightShaderClass()
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
	pixelShader = PixelShader::loadFromFile(L"DeferredDirectionalLightPixelShader.hlsl", &light_buffer_desc);
}

void DirectionalLightShaderClass::prepare(GameObject * object)
{
	DirectionalLight* lightComponent = object->getComponent<DirectionalLight>();
	if (lightComponent == nullptr)
		throw std::exception("The game object that has been given to the light shader to parse has no appropriate light component attached to it");

	// Update pixel shader constant buffer
	LightBuffer lightBufferValues;

	// Get direction from input object
	lightBufferValues.lightDirection = object->getTransform()->getWorldDirectionalVector();

	// Get light intensity from light object
	lightBufferValues.lightIntensity = lightComponent->getIntensity();

	lightBufferValues.lightColor = {
		lightComponent->getColor()[0],
		lightComponent->getColor()[1],
		lightComponent->getColor()[2],
		lightComponent->getColor()[3]
	};
	lightBufferValues.shininess = 64.0f;
	lightBufferValues.viewDirection = object->getScene()->getActiveCamera()->getGameObject()->getTransform()->getWorldDirectionalVector();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GraphicsManager::instance()->getDeviceContext()->Map(pixelShader->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &lightBufferValues, sizeof(LightBuffer));
	GraphicsManager::instance()->getDeviceContext()->Unmap(pixelShader->getConstantBuffer(), 0);

	// Set pixel Shader constant buffer
	ID3D11Buffer* pCB = getPixelShader()->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->PSSetConstantBuffers(0, 1, &pCB);
}
