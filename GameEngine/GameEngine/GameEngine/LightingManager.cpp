#pragma once

#include "LightingManager.h"
#include "Light.h"
#include "GraphicsManager.h"
#include "GameManager.h"
#include "Scene.h"
#include "AmbientLight.h"

LightingManager* LightingManager::s_instance = nullptr;

void LightingManager::render()
{
	// Set shader resources
	ID3D11ShaderResourceView* pSRV = GraphicsManager::instance()->getDeferredShaderResourceView(0);
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(0, 1, &pSRV); // Texture Data
	pSRV = GraphicsManager::instance()->getDeferredShaderResourceView(1);
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(1, 1, &pSRV); // Normal Data

	// Render ambient light first
	GameManager::instance()->getScene()->getAmbientLight()->render();

	// Enable blending
	float blendFactor[4] = { 0.0F, 0.0F, 0.0F, 0.0F };
	UINT sampleMask = 0x0F;
	GraphicsManager::instance()->getDeviceContext()
		->OMSetBlendState(GraphicsManager::instance()->getAdditiveBlendState(), blendFactor, sampleMask);

	// Render all active lights
	for (std::set<Light*>::const_iterator iterator = lights.begin(), end = lights.end(); iterator != end; ++iterator) {
		((Light*)*iterator)->render();
	}

	// Disable blending
	UINT defaultSampleMask = 0xffffffff;
	GraphicsManager::instance()->getDeviceContext()->OMSetBlendState(NULL, NULL, defaultSampleMask);

	// Clear Shader resources
	ID3D11ShaderResourceView *const pSRVNull[2] = { NULL, NULL };
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(0, 2, pSRVNull);
}

void LightingManager::registerLight(Light * light)
{
	lights.insert(light);
}

void LightingManager::deregisterLight(Light * light)
{
	lights.erase(light);
}
