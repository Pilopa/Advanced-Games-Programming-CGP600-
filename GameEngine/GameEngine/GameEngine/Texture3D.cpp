#pragma once

#include "DDSTextureLoader.h"
#include "Texture3D.h"
#include "GraphicsManager.h"

Texture3D::Texture3D(WCHAR * path)
{
	HRESULT hr;
	ID3D11Resource* textureResource;

	hr = DirectX::CreateDDSTextureFromFile(GraphicsManager::instance()->getDevice(), path, &textureResource, &textureView);
	
	if (FAILED(hr))
		throw std::exception("Texture3D could not be loaded!");

	textureResource->Release();
}
