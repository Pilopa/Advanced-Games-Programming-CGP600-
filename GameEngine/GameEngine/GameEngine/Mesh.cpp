#pragma once

#include "Mesh.h"
#include "GraphicsManager.h"
#include "Exceptions.h"
#include "Debug.h"

Mesh::Mesh(std::vector<VERTEX> vertices, std::vector<UINT> indices)
{
	primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	indexFormat = DXGI_FORMAT_R16_UINT;
	this->vertexCount = vertices.size();
	UINT sizeOfVertices = sizeof(vertices) + sizeof(VERTEX) * vertices.capacity();
	UINT sizeOfIndices = sizeof(indices) + sizeof(UINT) * indices.capacity();

	// Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU + GPU usage
	bufferDesc.ByteWidth = sizeOfVertices; // Buffersize
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Use created Buffer as Vertex Buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow CPU access

	if (FAILED(GraphicsManager::instance()->getDevice()->CreateBuffer(&bufferDesc, NULL, &vertexBuffer))) 
		throw std::exception("Mesh VertexBuffer could not be created");

	// Copy vertices to buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	// Lock the buffer to allow writing
	GraphicsManager::instance()->getDeviceContext()->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	// Copy the data
	memcpy(ms.pData, vertices.data(), sizeOfVertices);

	// Unlock the buffer
	GraphicsManager::instance()->getDeviceContext()->Unmap(vertexBuffer, NULL);

	// Set up and create index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeOfIndices;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	if (FAILED(GraphicsManager::instance()->getDevice()->CreateBuffer(&bufferDesc, &InitData, &indexBuffer)))
		throw std::exception("Mesh IndexBuffer could not be created");
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

DXGI_FORMAT Mesh::getIndexFormat()
{
	return indexFormat;
}

D3D11_PRIMITIVE_TOPOLOGY Mesh::getPrimitiveTopology()
{
	return primitiveTopology;
}
