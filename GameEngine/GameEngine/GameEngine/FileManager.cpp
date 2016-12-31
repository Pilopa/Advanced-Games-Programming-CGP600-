#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include "FileManager.h"
#include "GraphicsManager.h"
#include "WaveFrontReader.h"
#include "Debug.h"
#include "Vertex.h"
#include "Mesh.h"

std::map<std::wstring, DirectX::SpriteFont*> FileManager::fonts;

Mesh * FileManager::loadObjMesh(WCHAR * path)
{
	WaveFrontReader<UINT>* objReader = new WaveFrontReader<UINT>();
	objReader->Load(path);

	return new Mesh(objReader->vertices, objReader->indices);
}

DirectX::SpriteFont * FileManager::getFont(std::wstring name)
{
	return fonts.at(name);
}

DirectX::SpriteFont * FileManager::loadFont(std::wstring filename)
{
	fonts.insert_or_assign(filename, new DirectX::SpriteFont(GraphicsManager::instance()->getDevice(), filename.c_str()));
	return getFont(filename);
}