#pragma once

#include "GameObject.h"

using namespace DirectX;

class Prefab {

	public:
		virtual GameObject* instantiate(XMVECTOR position) = 0;

};