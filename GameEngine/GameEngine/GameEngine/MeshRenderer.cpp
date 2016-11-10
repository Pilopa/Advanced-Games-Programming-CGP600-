#pragma once

#include "GraphicsManager.h"
#include "MeshRenderer.h"
#include "ApplicationManager.h"
#include "Scene.h"

void MeshRenderer::update()
{
	// Do nothing
}

void MeshRenderer::draw()
{
	// Set input description
	GraphicsManager::instance()->getDeviceContext()->IASetInputLayout(material->getVertexShader()->getVertexInputLayout());

	// Set shaders
	GraphicsManager::instance()->getDeviceContext()->VSSetShader(material->getVertexShader()->getD3DVertexShaderInstance(), 0, 0);
	GraphicsManager::instance()->getDeviceContext()->PSSetShader(material->getPixelShader()->getD3DPixelShaderInstance(), 0, 0);

	// Set buffers
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	ID3D11Buffer* pVB = mesh->getVertexBuffer();
	GraphicsManager::instance()->getDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
	GraphicsManager::instance()->getDeviceContext()->IASetIndexBuffer(mesh->getIndexBuffer(), mesh->getIndexFormat(), 0);

	// Update vertex shader constant buffer
	DirectX::XMMATRIX world = getGameObject()->getTransform()->getWorldMatrix();
	DirectX::XMMATRIX view = getGameObject()->getScene()->getActiveCamera()->getViewMatrix();
	DirectX::XMMATRIX projection = getGameObject()->getScene()->getActiveCamera()->getProjectionMatrix(ApplicationManager::instance()->getWindow()->getAspectRatio());
	MatrixBuffer matrixBufferValues;
	matrixBufferValues.worldViewProjectionMatrix = world
								* view
								* projection;

	GraphicsManager::instance()->getDeviceContext()->UpdateSubresource(material->getVertexShader()->getConstantBuffer(), 0, 0, &matrixBufferValues, 0, 0);
	
	ID3D11Buffer* pCB = material->getVertexShader()->getConstantBuffer();
	GraphicsManager::instance()->getDeviceContext()->VSSetConstantBuffers(0, 1, &pCB);

	// Set primitive topology to use
	GraphicsManager::instance()->getDeviceContext()->IASetPrimitiveTopology(mesh->getPrimitiveTopology());

	// Perform rendering
	GraphicsManager::instance()->getDeviceContext()->Draw(mesh->getVertexCount(), 0);
}

MeshRenderer::MeshRenderer(Mesh * mesh, Material * material)
{
	this->mesh = mesh;
	this->material = material;
}
