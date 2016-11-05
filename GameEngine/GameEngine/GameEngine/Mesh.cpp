#include "Mesh.h"

ID3D11Buffer * Mesh::getVertexBuffer()
{
	return vertexBuffer;
}

ID3D11Buffer * Mesh::getIndexBuffer()
{
	return indexBuffer;
}

unsigned int Mesh::getVertexCount()
{
	return vertexCount;
}
