#pragma once

#include "GraphicsManager.h"
#include "UIManager.h"

UIManager* UIManager::s_instance = nullptr;

void UIManager::render()
{
	// Enable blending
	float blendFactor[4] = { 0.0F, 0.0F, 0.0F, 0.0F };
	UINT sampleMask = 0x0F;
	GraphicsManager::instance()->getDeviceContext()
		->OMSetBlendState(GraphicsManager::instance()->getAdditiveBlendState(), blendFactor, sampleMask);

	// Render all ui elements
	for (std::set<UIComponent*>::const_iterator iterator = uiComponents.begin(), end = uiComponents.end(); iterator != end; ++iterator) {
		((UIComponent*)*iterator)->render();
	}

	// Disable blending
	UINT defaultSampleMask = 0xffffffff;
	GraphicsManager::instance()->getDeviceContext()->OMSetBlendState(NULL, NULL, defaultSampleMask);
}

void UIManager::add(UIComponent * managable)
{
	uiComponents.insert(managable);
}

void UIManager::remove(UIComponent * managable)
{
	uiComponents.erase(managable);
}
