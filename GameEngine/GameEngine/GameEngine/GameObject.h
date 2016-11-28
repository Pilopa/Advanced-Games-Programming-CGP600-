#pragma once

#include <set>
#include "ClassDef.h"
#include "Transform.h"
#include "Utility.h"

class GameObject {

	public:

		GameObject();
		void update();
		void awake();
		void shutdown();

		Transform* getTransform();
		GameObject* getParent();
		void setParent(GameObject* parent);
		void addComponent(Component* component);
		Scene* getScene();
		void setScene(Scene* scene);

		template<typename T>
		T* getComponent()
		{
			//  Search components for one of given type
			for (std::set<Component*>::const_iterator iterator = components.begin(), end = components.end(); iterator != end; ++iterator) {
				if (instanceof<T, Component>((Component*)*iterator))
					return (T*)*iterator;
			}

			return nullptr;
		}

	private:

		Transform transform = Transform();
		GameObject* parent = nullptr;
		std::set<GameObject*> children;
		std::set<Component*> components;
		Scene* scene = nullptr;
};