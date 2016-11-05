#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include "Resource.h"

class Mesh : public Resource {

	public:
		ID3D11Buffer* getVertexBuffer();
		ID3D11Buffer* getIndexBuffer();
		unsigned int getVertexCount();

	private:
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		VERTEX* vertices;
		unsigned int vertexCount;

};