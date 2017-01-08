#pragma once

#include "Prefab.h"

#define PROJECTILE_MAT_SHININESS 5.0f

class ProjectilePrefab : public Prefab {

	public:
		ProjectilePrefab();
		GameObject* instantiate(XMVECTOR position); // Override Prefab::instantiate

		static ProjectilePrefab* instance() {
			if (!s_instance)
				s_instance = new ProjectilePrefab();
			return s_instance;
		}

	private:
		Mesh* projectileMesh;
		Texture* projectileTexture;
		Material* projectileMaterial;

		static ProjectilePrefab* s_instance;

};