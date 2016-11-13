/////////////
// GLOBALS //
/////////////
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);

///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypePoint : register(s0);

//////////////////////
// CONSTANT BUFFERS //
//////////////////////
cbuffer LightBuffer
{
	float3 lightDirection;
	float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	float4 colors;
	float4 normals;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	colors = colorTexture.Sample(SampleTypePoint, input.tex);

	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	normals = normalTexture.Sample(SampleTypePoint, input.tex);

	// Skip lighting calculations if pixel is unlit
	if (normals.r == 0.0f && normals.g == 0.0f &&
		normals.b == 0.0f && normals.a == 1.0f) return colors;

	float3 lightDir;
	float lightIntensity;
	float4 outputColor;

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(normals.xyz, lightDir));

	// Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.
	outputColor = saturate(colors * lightIntensity);

	return outputColor;

}