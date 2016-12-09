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
	float4 lightColor;
	float lightIntensity;
	float3 packaging_bytes;
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
	float4 input_normal;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	colors = colorTexture.Sample(SampleTypePoint, input.tex);
	float4 outputColor = colors;

	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	input_normal = normalTexture.Sample(SampleTypePoint, input.tex);

	if (!(input_normal.r == 0.0f && input_normal.g == 0.0f
		&& input_normal.b == 0.0f && input_normal.a == 0.0f)) {

		// Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.
		outputColor = saturate(lightIntensity * colors * lightColor);

	}

	return outputColor;

}