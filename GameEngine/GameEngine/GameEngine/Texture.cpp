#pragma once

#include "WICTextureLoader.h"
#include "Texture.h"
#include "Exceptions.h"
#include "GraphicsManager.h"

Texture::Texture(WCHAR * path)
{
	HRESULT hr;
	ID3D11Texture2D* textureResource;
	ID3D11Resource* resource;

	hr = DirectX::CreateWICTextureFromFile(GraphicsManager::instance()->getDevice(), path, &resource, &textureView);
	if (FAILED(hr))
		throw std::exception("Texture could not be loaded!");

	hr = resource->QueryInterface(&textureResource);
	if (FAILED(hr))
		throw std::exception("TextureResource could not be transformed to Texture");

	textureResource->GetDesc(&textureDesc);

	textureResource->Release();
	resource->Release();
}

ID3D11ShaderResourceView * Texture::getTextureView()
{
	return textureView;
}

D3D11_TEXTURE2D_DESC * Texture::getTextureDescription()
{
	return &textureDesc;
}

Texture::Texture()
{
	// Do nothing
}
