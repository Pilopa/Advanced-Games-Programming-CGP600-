#pragma once

#include <DirectXMath.h>
#include "ShaderClass.h"

class DirectionalLightShaderClass : public ShaderClass {

	public:
		DirectionalLightShaderClass();

		void prepare(GameObject* object); // Overrides ShaderClass::prepare

	private:

		struct LightBuffer
		{
			DirectX::XMFLOAT4 lightColor; // 4 * 4 bytes = 16 bytes
			DirectX::XMVECTOR lightDirection; // 4 * 4 bytes = 16 bytes
			DirectX::XMVECTOR viewDirection; // 4 * 4 bytes = 16 bytes
			float lightIntensity; // 4 bytes
			float shininess; // 4 bytes
		}; // = 48 bytes

};