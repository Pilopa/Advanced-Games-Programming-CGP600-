#pragma once

#include <DirectXMath.h>
#include "ShaderClass.h"

class PointLightShaderClass : public ShaderClass {

public:
	PointLightShaderClass();

	void prepare(GameObject* object); // Overrides ShaderClass::prepare

private:

	struct LightBuffer
	{
		DirectX::XMFLOAT4 lightColor; // 4 * 4 bytes = 16 bytes
		DirectX::XMVECTOR viewPosition; // 4 * 4 bytes = 16 bytes
		DirectX::XMVECTOR lightPosition; // 4 * 4 bytes = 16 bytes
		float lightIntensity; // 4 bytes
		float shininess; // 4 bytes
	}; // = 48 bytes

};