#pragma once

#include "Renderer.h"
#include "GraphicsManager.h"

void Renderer::awake()
{
	GraphicsManager::instance()->registerRenderer(this);
}

void Renderer::shutdown()
{
	GraphicsManager::instance()->deregisterRenderer(this);
}
