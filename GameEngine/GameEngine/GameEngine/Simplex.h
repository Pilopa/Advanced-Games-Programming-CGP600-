#pragma once

#include <vector>
#include <DirectXMath.h>
#include "ClassDef.h"
#include "Utility.h"

struct VectorEquals {

	const DirectX::XMVECTOR _vector;

	VectorEquals(DirectX::XMVECTOR vector) : _vector(vector) {}

	bool operator()(DirectX::XMVECTOR& elem)
	{
		return DirectX::XMVector3Equal(_vector, elem);
	}
};

struct Edge {

	DirectX::XMVECTOR normal = ZERO_VECTOR;
	DirectX::XMVECTOR distance = MAX_VECTOR;
	int index = 0;

};

class Simplex {

	public:
		static DirectX::XMVECTOR Support(Collider* a, Collider* b, DirectX::XMVECTOR direction);
		Edge getClosestEdge();
		void add(DirectX::XMVECTOR vector);
		bool containsOrigin(DirectX::XMVECTOR* outputDirection);
		DirectX::XMVECTOR last();
		DirectX::XMVECTOR A();
		DirectX::XMVECTOR B();
		DirectX::XMVECTOR C();
		void remove(DirectX::XMVECTOR vector);

	private:
		std::vector<DirectX::XMVECTOR> points;

};