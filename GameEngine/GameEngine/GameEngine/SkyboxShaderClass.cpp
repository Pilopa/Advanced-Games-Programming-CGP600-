#pragma once

#include "SkyboxShaderClass.h"
#include "GraphicsManager.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Scene.h"
#include "Camera.h"
#include "GameManager.h"
#include "Transform.h"

SkyboxShaderClass::SkyboxShaderClass()
{
	// Create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	constant_buffer_desc.ByteWidth = sizeof(MatrixBuffer); // Must be multiple of 16, calculate from CB struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as constant buffer
	constant_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constant_buffer_desc.MiscFlags = 0;
	constant_buffer_desc.StructureByteStride = 0;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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

	vertexShader = VertexShader::loadFromFile(L"DeferredSkyboxVertexShader.hlsl", &constant_buffer_desc);
	pixelShader = PixelShader::loadFromFile(L"DeferredSkyboxPixelShader.hlsl", nullptr, &samplerDesc);
}

void SkyboxShaderClass::prepare(GameObject* object)
{
	// Update vertex shader constant buffer
	MatrixBuffer matrixBufferValues;
	matrixBufferValues.worldMatrix = DirectX::XMMatrixTranspose(GameManager::instance()->getScene()->getActiveCamera()->getGameObject()->getTransform()->getWorldMatrix());
	matrixBufferValues.viewMatrix = DirectX::XMMatrixTranspose(GameManager::instance()->getScene()->getActiveCamera()->getViewMatrix());
	matrixBufferValues.projectionMatrix = DirectX::XMMatrixTranspose(GameManager::instance()->getScene()->getActiveCamera()->getProjectionMatrix(ApplicationManager::instance()->getWindow()->getAspectRatio()));
	matrixBufferValues.cameraPosition = GameManager::instance()->getScene()->getActiveCamera()->getGameObject()->getTransform()->getWorldPositionVector();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GraphicsManager::instance()->getDeviceContext()->Map(vertexShader->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &matrixBufferValues, sizeof(MatrixBuffer));
	GraphicsManager::instance()->getDeviceContext()->Unmap(vertexShader->getConstantBuffer(), 0);

	// Set Vertex Shader constant buffer
	ID3D11Buffer* pCB = getVertexShader()->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->VSSetConstantBuffers(0, 1, &pCB);
}
