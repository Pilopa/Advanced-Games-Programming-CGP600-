#pragma once

#include <DirectXMath.h>
#include "ShaderClass.h"

class AmbientLightShaderClass : public ShaderClass {

	public:
		AmbientLightShaderClass();

		void prepare(GameObject* object); // Overrides ShaderClass::prepare
		void prepare(AmbientLight* ambientLight);

	private:

		struct LightBuffer
		{
			DirectX::XMFLOAT4 lightColor; // 4x4 bytes = 16 bytes
			float lightIntensity; // 4 bytes
			DirectX::XMVECTOR packaging_bytes; // 3x4 bytes
		}; // = 32 bytes

};