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
	float4 lightDirection;
	float4 viewDirection;
	float lightIntensity;
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
	float4 input_color;
	float4 input_normal;
	float3 reflection;
	float4 specular_color = float4(1.0F, 1.0F, 1.0F, 1.0F);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 diffuse;
	float3 lightDir;
	float calculatedLightIntensity;
	float4 outputColor;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	input_color = colorTexture.Sample(SampleTypePoint, input.tex);

	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	input_normal = normalTexture.Sample(SampleTypePoint, input.tex);

	// Invert the light direction for calculations.
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
	calculatedLightIntensity = saturate(dot(input_normal.xyz, lightDir));

	// Set default output color
	outputColor = input_color;

	if (!(input_normal.r == 0.0f && input_normal.g == 0.0f
		&& input_normal.b == 0.0f && input_normal.a == 1.0f)) {

		// Calculate final diffuse color
		diffuse = saturate(lightIntensity * calculatedLightIntensity * lightColor * input_color);

		// Calculate reflection
		reflection = reflect(lightDir, input_normal.xyz);

		// Calculate specular
		specular = saturate(specular_color * pow(max(dot(reflection, viewDirection), 0.0F), 5.0f) * length(input_color));

		// Add specular
		outputColor = saturate(diffuse + specular);
		
	}

	return outputColor;

}