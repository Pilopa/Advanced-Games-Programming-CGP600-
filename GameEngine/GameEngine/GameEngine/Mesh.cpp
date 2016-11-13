#pragma once

#include "Mesh.h"
#include "GraphicsManager.h"
#include "Exceptions.h"
#include "Vertex.h"
#include "Debug.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
{
	initialize(vertices, indices, GraphicsManager::instance());
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, GraphicsManager * graphicsManager)
{
	initialize(vertices, indices, graphicsManager);
}

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

unsigned int Mesh::getIndexCount()
{
	return indexCount;
}

DXGI_FORMAT Mesh::getIndexFormat()
{
	return indexFormat;
}

D3D11_PRIMITIVE_TOPOLOGY Mesh::getPrimitiveTopology()
{
	return primitiveTopology;
}

void Mesh::release()
{
	vertexBuffer->Release();
	indexBuffer->Release();
}

void Mesh::initialize(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, GraphicsManager * graphicsManager)
{
	primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	indexFormat = DXGI_FORMAT_R32_UINT;
	this->vertexCount = vertices.size();
	this->indexCount = indices.size();
	UINT sizeOfVertices = sizeof(Vertex) * vertexCount;
	UINT sizeOfIndices = sizeof(UINT) * indexCount;

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU + GPU usage
	bufferDesc.ByteWidth = sizeOfVertices; // Buffersize
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use created Buffer as Vertex Buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU access

	if (FAILED(graphicsManager->getDevice()->CreateBuffer(&bufferDesc, NULL, &vertexBuffer)))
		throw std::exception("Mesh VertexBuffer could not be created");

	// Copy vertices to buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	// Lock the buffer to allow writing
	graphicsManager->getDeviceContext()->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// Copy the data
	memcpy(ms.pData, vertices.data(), sizeOfVertices);

	// Unlock the buffer
	graphicsManager->getDeviceContext()->Unmap(vertexBuffer, NULL);

	// Set up and create index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeOfIndices;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU access

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	if (FAILED(graphicsManager->getDevice()->CreateBuffer(&bufferDesc, &InitData, &indexBuffer)))
		throw std::exception("Mesh IndexBuffer could not be created");

	// Copy indices to buffer

	// Lock the buffer to allow writing
	graphicsManager->getDeviceContext()->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// Copy the data
	memcpy(ms.pData, indices.data(), sizeOfIndices);

	// Unlock the buffer
	graphicsManager->getDeviceContext()->Unmap(indexBuffer, NULL);
}
