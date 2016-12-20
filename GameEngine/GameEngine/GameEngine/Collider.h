#pragma once

#include <DirectXMath.h>
#include <set>
#include "ClassDef.h"
#include "Component.h"


class Collider : public Component {

	public: 
		void update(); // Overrides Component::update
		void awake(); // Overrides Component::awake
		void shutdown(); // Overrides Component::shutdown

		/* Each collision is immutable and specific to one collider.
			It is not going to be reused after being inserted into this method.
		*/
		virtual void onCollision(const Collision* collision) = 0;
		virtual std::set<DirectX::XMVECTOR>* checkCollision(Collider* other) = 0;

};