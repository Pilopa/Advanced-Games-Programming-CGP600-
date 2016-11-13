//////////////
// TEXTURES //
//////////////
Texture2D shaderTexture : register(t0);

///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypeWrap : register(s0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
PixelOutputType main(PixelInputType input)
{
	PixelOutputType output;

	// Sample the color from the texture and store it for output to the render target.
	output.color = shaderTexture.Sample(SampleTypeWrap, input.texCoord);

	// Store the normal for output to the render target.
	output.normal = float4(input.normal, 0.0f);

	return output;
}