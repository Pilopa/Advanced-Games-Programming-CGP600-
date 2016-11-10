#pragma once

#include <vector>
#include <d3d11.h>
#include "Vertex.h"
#include "Resource.h"

class Mesh : public Resource {

	public:
		Mesh(std::vector<VERTEX> vertices, std::vector<UINT> indices);
		ID3D11Buffer* getVertexBuffer();
		ID3D11Buffer* getIndexBuffer();
		unsigned int getVertexCount();
		DXGI_FORMAT getIndexFormat();
		D3D11_PRIMITIVE_TOPOLOGY getPrimitiveTopology();

	private:
		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		DXGI_FORMAT indexFormat;
		unsigned int vertexCount;

};