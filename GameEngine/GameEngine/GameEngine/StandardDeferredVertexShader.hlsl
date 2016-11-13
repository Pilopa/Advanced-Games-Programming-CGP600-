//////////////////////
// CONSTANT BUFFERS //
//////////////////////

cbuffer MatrixBuffer {
	matrix worldMatrix; // = 64 bytes
	matrix viewMatrix; // = 64 bytes
	matrix projectionMatrix; // = 64 bytes
}; 

   //////////////
   // TYPEDEFS //
   //////////////
struct VertexInputType
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	float4 pos = float4(input.position, 1.0F);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(pos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.texCoord = input.texCoord;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//Normalize normal vector
	output.normal = normalize(output.normal);

	return output;
}