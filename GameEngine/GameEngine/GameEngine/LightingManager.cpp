#pragma once

#include "LightingManager.h"
#include "Light.h"
#include "GraphicsManager.h"

LightingManager* LightingManager::s_instance = nullptr;

void LightingManager::render()
{
	// Set shader resources
	ID3D11ShaderResourceView* pSRV = GraphicsManager::instance()->getDeferredShaderResourceView(0);
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(0, 1, &pSRV); // Texture Data
	pSRV = GraphicsManager::instance()->getDeferredShaderResourceView(1);
	GraphicsManager::instance()->getDeviceContext()->PSSetShaderResources(1, 1, &pSRV); // Normal Data

	// Render all active lights
	for (std::set<Light*>::const_iterator iterator = lights.begin(), end = lights.end(); iterator != end; ++iterator) {
		((Light*)*iterator)->render();
	}

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
