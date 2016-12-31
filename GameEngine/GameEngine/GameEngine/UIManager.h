#pragma once

#include <set>
#include "Manager.h"
#include "UIComponent.h"

class UIManager : public Manager<UIComponent> {

	public:
		void render();
		void add(UIComponent* managable);
		void remove(UIComponent* managable);

		static UIManager* instance() {
			if (!s_instance)
				s_instance = new UIManager();
			return s_instance;
		}

	private:
		static UIManager* s_instance;

		std::set<UIComponent*> uiComponents;
};