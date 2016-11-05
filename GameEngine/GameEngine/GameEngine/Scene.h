#pragma once

#include <set>
#include "GameObject.h"
#include "Camera.h"

class Scene {

	public:

		void update();
		void awake();

		void addRootObject(GameObject* object);
		void setActiveCamera(Camera* camera);

	private:

		std::set<GameObject*> rootGameObjects;
		Camera* activeCamera;

};