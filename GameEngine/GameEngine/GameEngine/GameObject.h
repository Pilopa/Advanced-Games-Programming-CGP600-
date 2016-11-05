#pragma once

#include <set>
#include "Component.h"

class GameObject {

	public:

		GameObject();
		void update();
		void awake();

		GameObject* getParent();
		void setParent(GameObject* parent);

	private:

		GameObject* parent;
		std::set<GameObject*> children;
		std::set<Component*> components;
};
