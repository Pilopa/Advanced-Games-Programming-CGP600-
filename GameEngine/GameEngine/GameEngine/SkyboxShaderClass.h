#pragma once

#include <DirectXMath.h>
#include "ShaderClass.h"

class SkyboxShaderClass : public ShaderClass {

	public:
		SkyboxShaderClass();

		void prepare(GameObject* object = nullptr); // Overrides ShaderClass::prepare

	private:

		struct MatrixBuffer {
			DirectX::XMMATRIX worldMatrix; // 64 bytes
			DirectX::XMMATRIX viewMatrix; // 64 bytes
			DirectX::XMMATRIX projectionMatrix; // 64 bytes
			DirectX::XMVECTOR cameraPosition; // 16 bytes
		};
};