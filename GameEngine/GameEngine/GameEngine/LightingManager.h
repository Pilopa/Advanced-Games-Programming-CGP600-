#pragma once

#include <set>
#include "ClassDef.h"

class LightingManager {

	public:

		void render();
		void registerLight(Light* light);
		void deregisterLight(Light* light);

		static LightingManager* instance() {
			if (!s_instance)
				s_instance = new LightingManager();
			return s_instance;
		}
		 
	private:

		std::set<Light*> lights;

		static LightingManager* s_instance;
};