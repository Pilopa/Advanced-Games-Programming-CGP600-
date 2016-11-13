#pragma once

#include <vector>
#include <d3d11.h>
#include "Resource.h"
#include "ClassDef.h"

class Mesh : public Resource {

	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, GraphicsManager* graphicsManager);
		ID3D11Buffer* getVertexBuffer();
		ID3D11Buffer* getIndexBuffer();
		unsigned int getVertexCount();
		unsigned int getIndexCount();
		DXGI_FORMAT getIndexFormat();
		D3D11_PRIMITIVE_TOPOLOGY getPrimitiveTopology();
		void release();

	private:
		D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		DXGI_FORMAT indexFormat;
		unsigned int vertexCount;
		unsigned int indexCount;
		void initialize(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, GraphicsManager* graphicsManager);

};