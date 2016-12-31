#pragma once

#include "UIComponent.h"
#include "ClassDef.h"

class ImageComponent : public UIComponent {

	public:
		ImageComponent(DirectX::SimpleMath::Vector2 position, Texture* texture);
		void render(); // Overrides UIComponent::render

	private:
		Texture* texture;

};