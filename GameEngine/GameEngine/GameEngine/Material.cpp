#include "Material.h"

Material::Material(ShaderClass * shaderClass, Texture * texture)
{
	this->shaderClass = shaderClass;
	this->texture = texture;
}

ShaderClass * Material::getShaderClass()
{
	return shaderClass;
}

Texture * Material::getTexture()
{
	return texture;
}
