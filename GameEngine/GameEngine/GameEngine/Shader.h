#pragma once

#include <d3d11.h>

class Shader {

	public:
		ID3D11Buffer* getConstantBuffer();
	
	protected:
		ID3D11Buffer* constantBuffer = nullptr;
		
};