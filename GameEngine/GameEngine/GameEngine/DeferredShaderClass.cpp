#pragma once

#include "DeferredShaderClass.h"
#include "GraphicsManager.h"
#include "ApplicationManager.h"
#include "GameObject.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Scene.h"
#include "Camera.h"

DeferredShaderClass* DeferredShaderClass::INSTANCE;

DeferredShaderClass::DeferredShaderClass()
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

	vertexShader = VertexShader::loadFromFile(L"StandardDeferredVertexShader.hlsl", &constant_buffer_desc);
	pixelShader = PixelShader::loadFromFile(L"StandardDeferredPixelShader.hlsl");
}

void DeferredShaderClass::prepare(GameObject* object)
{
	// Update vertex shader constant buffer
	MatrixBuffer matrixBufferValues;
	matrixBufferValues.worldMatrix = DirectX::XMMatrixTranspose(object->getTransform()->getWorldMatrix());
	matrixBufferValues.viewMatrix = DirectX::XMMatrixTranspose(object->getScene()->getActiveCamera()->getViewMatrix());
	matrixBufferValues.projectionMatrix = DirectX::XMMatrixTranspose(object->getScene()->getActiveCamera()->getProjectionMatrix(ApplicationManager::instance()->getWindow()->getAspectRatio()));

	//GraphicsManager::instance()->getDeviceContext()->UpdateSubresource(vertexShader->getConstantBuffer(), 0, 0, &matrixBufferValues, 0, 0);
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GraphicsManager::instance()->getDeviceContext()->Map(vertexShader->getConstantBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &matrixBufferValues, sizeof(MatrixBuffer));
	GraphicsManager::instance()->getDeviceContext()->Unmap(vertexShader->getConstantBuffer(), 0);
	
	// Set Vertex Shader constant buffer
	ID3D11Buffer* pCB = getVertexShader()->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->VSSetConstantBuffers(0, 1, &pCB);
}
