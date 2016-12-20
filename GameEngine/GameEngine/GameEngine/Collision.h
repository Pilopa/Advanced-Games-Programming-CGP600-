#pragma once

#include "ClassDef.h"
#include <DirectXMath.h>
#include <set>

class Collision {

	public:
		Collision(const Collider* other, std::set<DirectX::XMVECTOR> pointOfImpact);

		const Collider* getOther();
		const std::set<DirectX::XMVECTOR>* getPointsOfImpact();

	private:
		const Collider* other;
		const std::set<DirectX::XMVECTOR>* pointsOfImpact;

};