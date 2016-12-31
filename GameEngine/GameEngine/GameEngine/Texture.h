#pragma once

#include <d3d11.h>
#include "Resource.h"

class Texture : public Resource {

	public:
		Texture(WCHAR* path);

		ID3D11ShaderResourceView* getTextureView();
		D3D11_TEXTURE2D_DESC* getTextureDescription();

	protected:
		Texture();

		ID3D11ShaderResourceView* textureView = nullptr;
		D3D11_TEXTURE2D_DESC textureDesc;
};