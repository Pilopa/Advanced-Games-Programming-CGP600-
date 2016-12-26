#pragma once

#include <DirectXMath.h>
#include <set>
#include <map>
#include "ClassDef.h"
#include "Manager.h"

class CollisionManager : public Manager<Collider> {

	public:
		void performCollisionCheck();

		void add(Collider* collider); // Overrides Manager::add
		void remove(Collider* collider); // Overrides Manager::remove

		static CollisionManager* instance() {
			if (!s_instance)
				s_instance = new CollisionManager();
			return s_instance;
		}

	private:
		std::map<Collider*, DirectX::XMMATRIX> colliders;

		void forCollider(Collider* collider);

		static CollisionManager* s_instance;
};