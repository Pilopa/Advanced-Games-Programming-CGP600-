#pragma once

#include <DirectXMath.h>
#include "Utility.h"
#include "ClassDef.h"

using namespace DirectX;

class Collision {

	public:
		Collision(Collider* other, XMVECTOR minimumTranslationVector, float penetrationAmount);

		Collider* getOther();
		XMVECTOR getMinimumTranslationVector();
		float getPenetrationAmount();

	private:
		Collider* other;
		XMVECTOR minimumTranslationVector;
		float penetrationAmount;

};