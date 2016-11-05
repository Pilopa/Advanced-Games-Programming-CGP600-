#pragma once

#include "GraphicsManager.h"
#include "MeshRenderer.h"

void MeshRenderer::update()
{
	// Do nothing
}

void MeshRenderer::awake()
{
	// Do nothing
}

void MeshRenderer::draw()
{
	// Set shaders
	GraphicsManager::instance()->getDeviceContext()->VSSetShader(material->getVertexShader()->getD3DVertexShaderInstance(), 0, 0);
	GraphicsManager::instance()->getDeviceContext()->PSSetShader(material->getPixelShader()->getD3DPixelShaderInstance(), 0, 0);

	// Perform rendering
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	ID3D11Buffer* pVB = mesh->getVertexBuffer();
	GraphicsManager::instance()->getDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
	GraphicsManager::instance()->getDeviceContext()->Draw(mesh->getVertexCount(), 0);
}
