#include "Material.h"

Material::Material(ShaderClass * shaderClass, Texture * texture, float shininess)
{
	this->shaderClass = shaderClass;
	this->texture = texture;
	this->shininess = shininess;
}

ShaderClass * Material::getShaderClass()
{
	return shaderClass;
}

Texture * Material::getTexture()
{
	return texture;
}

float Material::getShininess()
{
	return shininess;
}
