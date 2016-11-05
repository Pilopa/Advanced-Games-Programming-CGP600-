#include "Material.h"

VertexShader * Material::getVertexShader()
{
	return vertexShader;
}

PixelShader * Material::getPixelShader()
{
	return pixelShader;
}
