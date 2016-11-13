#pragma once

#include <DirectXTex.h>
#include "WICTextureLoader.h"
#include "Texture.h"
#include "Exceptions.h"
#include "GraphicsManager.h"

Texture::Texture(WCHAR * path)
{
	HRESULT hr;
	ID3D11Resource* textureResource;

	hr = DirectX::CreateWICTextureFromFile(GraphicsManager::instance()->getDevice(), path, &textureResource, &textureView);
	if (FAILED(hr))
		throw std::exception("Texture could not be loaded!");

	textureResource->Release();
}

ID3D11ShaderResourceView * Texture::getTextureView()
{
	return textureView;
}
