#pragma once

#include <set>
#include "ClassDef.h"
#include "Transform.h"

class GameObject {

	public:

		GameObject();
		void update();
		void awake();

		Transform* getTransform();
		GameObject* getParent();
		void setParent(GameObject* parent);
		void addComponent(Component* component);
		Scene* getScene();
		void setScene(Scene* scene);

	private:

		Transform transform = Transform();
		GameObject* parent = nullptr;
		std::set<GameObject*> children;
		std::set<Component*> components;
		Scene* scene = nullptr;
};
