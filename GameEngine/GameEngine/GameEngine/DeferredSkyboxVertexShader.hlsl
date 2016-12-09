//////////////////////
// CONSTANT BUFFERS //
//////////////////////

cbuffer MatrixBuffer {
	matrix worldMatrix; // 64 bytes
	matrix viewMatrix; // = 64 bytes
	matrix projectionMatrix; // = 64 bytes
	float4 cameraPosition; // = 16 bytes
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	float3 rotatedPosition = mul(input.position, viewMatrix);
	output.position = mul(float4(rotatedPosition, 1.0f), projectionMatrix).xyww;

	output.texCoord = input.position;

	return output;
}