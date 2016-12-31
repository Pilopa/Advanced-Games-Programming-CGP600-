#pragma once

#include <map>
#include "SpriteFont.h"
#include "ClassDef.h"

class FileManager {

	public:
		static Mesh* loadObjMesh(WCHAR* path);
		static DirectX::SpriteFont* getFont(std::wstring name);
		static DirectX::SpriteFont* loadFont(std::wstring filename);

	private:
		static std::map<std::wstring, DirectX::SpriteFont*> fonts;

};