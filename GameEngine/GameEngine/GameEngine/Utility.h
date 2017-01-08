#pragma once

// Tests if the given pointer points to a subclass of the given Base class
template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

/* Compares each of the entries in the given matrices and returns 
 	if every one is equal.
 */
inline bool areEqual(const DirectX::XMMATRIX* m1, const DirectX::XMMATRIX* m2) {
	for (int x = 0; x < 4; x++) 
		for (int y = 0; y < 4; y++) 
			if (m1->r[x].m128_f32[y] != m2->r[x].m128_f32[y])
				return false;

	return true;
}

struct VectorCompare
{
	bool operator() (const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b) {
		for (int x = 0; x < 4; x++)
			if (a.m128_f32[x] != b.m128_f32[x])
				return false;

		return true;
	}
};

#define TripleProduct(a, b, c) DirectX::XMVector3Cross(DirectX::XMVector3Cross(a, b), c)
#define Vector(value) { value, value, value, 1.0f }
#define XVector { 1.0f, 0.0f, 0.0f, 1.0f }
#define YVector { 0.0f, 1.0f, 0.0f, 1.0f }
#define ZVector { 0.0f, 0.0f, 1.0f, 1.0f }

// === Define unique vectors ===

#define ZERO_VECTOR Vector(0.0f)
#define MAX_VECTOR Vector(FLT_MAX)