#pragma once

#include <DirectXMath.h>
#include "ShaderClass.h"

class DeferredShaderClass : public ShaderClass {

	public:
		DeferredShaderClass();

		static DeferredShaderClass* INSTANCE;

		void prepare(GameObject* object); // Overrides ShaderClass::prepare

	private:

		struct MatrixBuffer {
			DirectX::XMMATRIX worldMatrix; // 64 bytes
			DirectX::XMMATRIX viewMatrix; // 64 bytes
			DirectX::XMMATRIX projectionMatrix; // 64 bytes
		};

};