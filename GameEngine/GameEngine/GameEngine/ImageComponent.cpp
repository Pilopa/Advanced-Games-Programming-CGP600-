#pragma once

#include <memory>
#include "ImageComponent.h"
#include "GraphicsManager.h"
#include "Texture.h"

ImageComponent::ImageComponent(DirectX::SimpleMath::Vector2 position, Texture * texture) : UIComponent(position)
{
	this->texture = texture;
}

void ImageComponent::render()
{
	D3D11_TEXTURE2D_DESC* desc;
	desc = texture->getTextureDescription();

	auto origin = DirectX::SimpleMath::Vector2();
	origin.x = float(desc->Width / 2);
	origin.y = float(desc->Height / 2);

	DirectX::SpriteBatch* spriteBatch = GraphicsManager::instance()->getSpriteBatch();

	spriteBatch->Begin();

	spriteBatch->Draw(texture->getTextureView(), position, nullptr, DirectX::Colors::White, 0.f, origin);

	spriteBatch->End();
}
