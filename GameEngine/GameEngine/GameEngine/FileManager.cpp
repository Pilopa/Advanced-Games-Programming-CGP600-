#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include "WaveFrontReader.h"
#include "FileManager.h"
#include "Debug.h"
#include "Vertex.h"
#include "Mesh.h"

Mesh * FileManager::loadObjMesh(WCHAR * path)
{

	WaveFrontReader<UINT>* objReader = new WaveFrontReader<UINT>();
	objReader->Load(path);

	return new Mesh(objReader->vertices, objReader->indices);
}