#pragma once

#include "UIComponent.h"
#include "UIManager.h"

void UIComponent::update()
{
	// Do nothing
}

void UIComponent::awake()
{
	UIManager::instance()->add(this);
}

void UIComponent::shutdown()
{
	UIManager::instance()->remove(this);
}

UIComponent::UIComponent(DirectX::SimpleMath::Vector2 position)
{
	this->position = position;
}
