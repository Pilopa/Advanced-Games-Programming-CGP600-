#pragma once

#include <set>
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
		bool containsOrigin(DirectX::XMVECTOR* outputDirection);
		std::set<DirectX::XMVECTOR, VectorCompare>* check();

		DirectX::XMVECTOR last();
		DirectX::XMVECTOR A();
		DirectX::XMVECTOR B();
		DirectX::XMVECTOR C();

		void add(DirectX::XMVECTOR vector);
		void remove(DirectX::XMVECTOR vector);
		
		Simplex(Collider* a, Collider* b, DirectX::XMVECTOR initialDirection);

	private:
		std::vector<DirectX::XMVECTOR> points;
		Collider* cA;
		Collider* cB;
		DirectX::XMVECTOR direction;

};