#pragma once

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}

/* Compares each of the entries in the given matrices and returns 
 	if every one is equal.
 */
inline bool areEqual(const DirectX::XMMATRIX* m1, const DirectX::XMMATRIX* m2) {
	DirectX::XMFLOAT4X4 fm1, fm2;
	DirectX::XMStoreFloat4x4(&fm1, *m1);
	DirectX::XMStoreFloat4x4(&fm2, *m2);
	for (int x = 0; x < 4; x++) 
		for (int y = 0; y < 4; y++) 
			if (fm1.m[x,y] != fm2.m[x, y])
				return false;

	return true;
}