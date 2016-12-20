#pragma once

#include <DirectXMath.h>
#include <set>
#include <map>
#include "ClassDef.h"

class CollisionManager {

	public:
		void performCollisionCheck();

		void registerCollider(Collider* collider);
		void deregisterCollider(Collider* collider);

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