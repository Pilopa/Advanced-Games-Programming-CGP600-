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
	for (int i = 0; i < BUFFER_COUNT; i++) {
		ID3D11ShaderResourceView* pSRV = GraphicsManager::instance()->getDeferredShaderResourceView(i);
		GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(i, 1, &pSRV);
	}

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
	ID3D11ShaderResourceView* pSRVNull[BUFFER_COUNT];
	for (int i = 0; i < BUFFER_COUNT; i++) {
		pSRVNull[i] = NULL;
	}
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(0, BUFFER_COUNT, pSRVNull);
}

void LightingManager::registerLight(Light * light)
{
	lights.insert(light);
}

void LightingManager::deregisterLight(Light * light)
{
	lights.erase(light);
}
