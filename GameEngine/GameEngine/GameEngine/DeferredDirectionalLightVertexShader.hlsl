//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	output.position = float4(input.position, 1.0F);

	// Store the texture coordinates for the pixel shader.
	output.texCoord = input.texCoord;

	return output;
}