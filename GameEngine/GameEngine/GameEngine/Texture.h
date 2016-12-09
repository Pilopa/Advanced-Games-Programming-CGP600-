#pragma once

#include <d3d11.h>
#include "Resource.h"

class Texture : public Resource {

	public:
		Texture(WCHAR* path);

		ID3D11ShaderResourceView* getTextureView();

	protected:
		Texture();

		ID3D11ShaderResourceView* textureView;
};