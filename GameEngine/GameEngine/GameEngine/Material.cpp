#include "Material.h"

Material::Material(VertexShader * vertexShader, PixelShader * pixelShader, Texture * texture)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
	this->texture = texture;
}

VertexShader * Material::getVertexShader()
{
	return vertexShader;
}

PixelShader * Material::getPixelShader()
{
	return pixelShader;
}
