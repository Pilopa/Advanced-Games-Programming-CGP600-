#pragma once

#include <DirectXMath.h>
#include <set>
#include "Utility.h"
#include "ClassDef.h"

class Collision {

	public:
		Collision(Collider* other, std::set<DirectX::XMVECTOR, VectorCompare>* pointOfImpact);

		Collider* getOther();
		std::set<DirectX::XMVECTOR, VectorCompare>* getPointsOfImpact();

	private:
		Collider* other;
		std::set<DirectX::XMVECTOR, VectorCompare>* pointsOfImpact;

};