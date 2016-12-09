#pragma once

#include <set>
#include "ClassDef.h"

class Scene {

	public:

		void update();
		void awake();
		void shutdown();

		void addRootObject(GameObject* object);
		void setActiveCamera(Camera* camera);
		Camera* getActiveCamera();
		void setAmbientLight(AmbientLight* ambientLight);
		AmbientLight* getAmbientLight();
		Renderer* getSkybox();

		Scene();

	private:

		std::set<GameObject*> rootGameObjects;
		Camera* activeCamera = nullptr;
		AmbientLight* ambientLight = nullptr;
		Renderer* skybox = nullptr;

};