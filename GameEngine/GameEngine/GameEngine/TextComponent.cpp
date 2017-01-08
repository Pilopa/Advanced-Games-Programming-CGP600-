#pragma once

#include "TextComponent.h"
#include "GraphicsManager.h"
#include "Utility.h"

TextComponent::TextComponent(DirectX::SimpleMath::Vector2 position, DirectX::SpriteFont * font, int fontSize, DirectX::XMVECTOR color, std::wstring text) : UIComponent(position)
{
	this->font = font;
	this->fontSize = fontSize;
	this->color = color;
	this->text = text;
}

void TextComponent::render()
{
	DirectX::SpriteBatch* spriteBatch = GraphicsManager::instance()->getSpriteBatch();

	spriteBatch->Begin();

	DirectX::SimpleMath::Vector2 origin = DirectX::XMVectorDivide(font->MeasureString(text.c_str()), Vector(2.f));

	font->DrawString(spriteBatch, text.c_str(), position, color, 0.f, origin, (float) fontSize / REFERENCE_FONT_SIZE);

	spriteBatch->End();
}
