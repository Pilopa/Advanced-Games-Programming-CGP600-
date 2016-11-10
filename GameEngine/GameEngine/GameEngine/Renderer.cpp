#pragma once

#include "Renderer.h"
#include "GraphicsManager.h"

void Renderer::awake()
{
	GraphicsManager::instance()->registerRenderer(this);
}
