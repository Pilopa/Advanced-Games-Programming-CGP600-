#pragma once

#include <memory>
#include <string>
#include "SpriteFont.h"
#include "SimpleMath.h"
#include "UIComponent.h"

#define REFERENCE_FONT_SIZE 16.0f

class TextComponent : public UIComponent {

	public:
		TextComponent(DirectX::SimpleMath::Vector2 position, DirectX::SpriteFont* font, int fontSize, DirectX::XMVECTOR color, std::wstring text);
		void render(); // Overrides UIComponent::render

	private:
		DirectX::XMVECTOR color = DirectX::Colors::Black;
		DirectX::SpriteFont* font;
		std::wstring text = L"Text";
		int fontSize = REFERENCE_FONT_SIZE;

};