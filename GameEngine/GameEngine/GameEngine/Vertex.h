#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexCoord;
	DirectX::XMFLOAT3 Normal;
};

struct xyz { float x, y, z; };	//used for vertices and normals during file parse
struct xy { float x, y; };		//used for texture coordinates during file parse