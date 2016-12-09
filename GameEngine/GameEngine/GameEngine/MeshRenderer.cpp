#pragma once

#include "GraphicsManager.h"
#include "MeshRenderer.h"
#include "ApplicationManager.h"
#include "Scene.h"
#include "Vertex.h"
#include "ShaderClass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "Debug.h"

void MeshRenderer::update()
{
	// Do nothing
}

void MeshRenderer::draw()
{
	// Set input description
	GraphicsManager::instance()->getDeviceContext()->IASetInputLayout(material->getShaderClass()->getVertexShader()->getVertexInputLayout());

	// Set shaders
	GraphicsManager::instance()->getDeviceContext()->VSSetShader(material->getShaderClass()->getVertexShader()->getD3DVertexShaderInstance(), 0, 0);
	GraphicsManager::instance()->getDeviceContext()->PSSetShader(material->getShaderClass()->getPixelShader()->getD3DPixelShaderInstance(), 0, 0);

	// Set the sampler state in the pixel shader.
	ID3D11SamplerState* pSS = material->getShaderClass()->getPixelShader()->getSamplerState();
	GraphicsManager::instance()->getDeviceContext()->PSSetSamplers(0, 1, &pSS);

	// Set vertex and index buffers
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* pVB = mesh->getVertexBuffer();
	GraphicsManager::instance()->getDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
	GraphicsManager::instance()->getDeviceContext()->IASetIndexBuffer(mesh->getIndexBuffer(), mesh->getIndexFormat(), 0);

	// Prepare Shader buffers if possible
	if (getGameObject()) material->getShaderClass()->prepare(getGameObject());
	else LogError("Component is not attached to a game object!");

	// Setup texture for shader
	ID3D11ShaderResourceView* pSRV = material->getTexture()->getTextureView();
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(0, 1, &pSRV); // Texture

	// Set primitive topology to use
	GraphicsManager::instance()->getDeviceContext()->IASetPrimitiveTopology(mesh->getPrimitiveTopology());

	// Perform rendering
	GraphicsManager::instance()->getDeviceContext()->DrawIndexed(mesh->getIndexCount(), 0, 0);
}

MeshRenderer::MeshRenderer(Mesh * mesh, Material * material)
{
	this->mesh = mesh;
	this->material = material;
}

Material * MeshRenderer::getMaterial()
{
	return material;
}
