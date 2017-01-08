#pragma once

#include <vector>
#include "ClassDef.h"

class SceneLoader {

	public:
		static void setupScenes(DeferredShaderClass* deferredShaderClass);
		static Scene* loadScene(int index);

	private:
		static std::vector<Scene*> scenes;
};