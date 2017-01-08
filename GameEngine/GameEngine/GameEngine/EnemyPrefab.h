#pragma once

#include "Prefab.h"

#define ENEMY_MAT_SHININESS 5.0f

class EnemyPrefab : public Prefab {

	public:
		EnemyPrefab();
		GameObject* instantiate(XMVECTOR position); // Override Prefab::instantiate

		static EnemyPrefab* instance() {
			if (!s_instance)
				s_instance = new EnemyPrefab();
			return s_instance;
		}

	private:
		Mesh* mesh;
		Texture* texture;
		Material* material;

		static EnemyPrefab* s_instance;

};