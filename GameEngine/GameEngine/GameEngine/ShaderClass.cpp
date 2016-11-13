#pragma once

#include "ShaderClass.h"

VertexShader * ShaderClass::getVertexShader()
{
	return vertexShader;
}

PixelShader * ShaderClass::getPixelShader()
{
	return pixelShader;
}