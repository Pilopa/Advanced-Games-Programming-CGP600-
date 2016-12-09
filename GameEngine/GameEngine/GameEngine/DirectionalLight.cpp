#pragma once

#include "DirectionalLight.h"
#include "GraphicsManager.h"
#include "DirectionalLightShaderClass.h"
#include "VertexShader.h"
#include "PixelShader.h"

DirectionalLight::DirectionalLight(float color[4], float intensity) : Light(color, intensity)
{
	shaderClass = new DirectionalLightShaderClass();
}

void DirectionalLight::render()
{
	int indexCount = RENDER_QUAD_VERTEX_COUNT;

	// Prepare Buffers depending on shader class implementation
	getShaderClass()->prepare(getGameObject());

	// Set the vertex input layout.
	GraphicsManager::instance()->getDeviceContext()->IASetInputLayout(getShaderClass()->getVertexShader()->getVertexInputLayout());

	// Set the vertex and pixel shaders that will be used to render.
	GraphicsManager::instance()->getDeviceContext()->VSSetShader(getShaderClass()->getVertexShader()->getD3DVertexShaderInstance(), NULL, 0);
	GraphicsManager::instance()->getDeviceContext()->PSSetShader(getShaderClass()->getPixelShader()->getD3DPixelShaderInstance(), NULL, 0);

	// Set the sampler state in the pixel shader.
	ID3D11SamplerState* pSS = getShaderClass()->getPixelShader()->getSamplerState();
	GraphicsManager::instance()->getDeviceContext()->PSSetSamplers(0, 1, &pSS);

	// Render the geometry.
	GraphicsManager::instance()->getDeviceContext()->DrawIndexed(indexCount, 0, 0);
}