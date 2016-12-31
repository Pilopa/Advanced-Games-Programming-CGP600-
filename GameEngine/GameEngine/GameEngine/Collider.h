#pragma once

#include <DirectXMath.h>
#include <set>
#include "ClassDef.h"
#include "Component.h"
#include "Manager.h"
#include "Utility.h"

class Collider : public Component {

	public: 
		Collider(Manager<Collider>* manager, DirectX::XMVECTOR centerOffset, bool isTrigger);

		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void shutdown(); // Overrides Component::shutdown

		/* Each collision is immutable and specific to one collider.
			It is not going to be reused after being inserted into this method.
		*/
		virtual void onCollision(Collision* collision) = 0;
		virtual std::set<DirectX::XMVECTOR, VectorCompare>* checkCollision(Collider* other) = 0;
		virtual DirectX::XMVECTOR getFarthestVertexInDirection(DirectX::XMVECTOR direction);

		DirectX::XMVECTOR getCenterOffset();
		bool isTrigger();

	private:
		Manager<Collider>* manager;
		DirectX::XMVECTOR centerOffset = { 0.0F, 0.0F, 0.0F };
		bool trigger = false;
		
};