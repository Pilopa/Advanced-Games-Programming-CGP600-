#pragma once

#include "ClassDef.h"

class Component {

	public:
		GameObject* getGameObject();
		void setGameObject(GameObject* gameObject);

		virtual void update() = 0;
		virtual void awake() = 0;
		virtual void shutdown() = 0;
		
	private:
		GameObject* gameObject = nullptr;
};