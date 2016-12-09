/////////////
// GLOBALS //
/////////////
Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D positionTexture : register(t2);

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
	float shininess;
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
	float4 input_position;
	float4 outputColor;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	input_color = colorTexture.Sample(SampleTypePoint, input.tex);

	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	input_normal = normalTexture.Sample(SampleTypePoint, input.tex);

	// Sample the position from the position render texture using the point sampler at this texture coordinate location.
	input_position = positionTexture.Sample(SampleTypePoint, input.tex);

	// Calculate view vector
	float3 V = -viewDirection.xyz;

	// Invert the light direction for calculations.
	float3 L = -lightDirection.xyz;

	// Calculate the amount of light on this pixel.
	float NdotL = saturate(dot(input_normal.xyz, L));

	// Set default output color
	outputColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (!(input_normal.r == 0.0f && input_normal.g == 0.0f
		&& input_normal.b == 0.0f && input_normal.a == 0.0f)) {

		float4 specular_color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		// Calculate the light's diffuse color
		float4 diffuse = saturate(lightColor * NdotL);

		// Calculate reflection
		float3 R = normalize(reflect(-L, input_normal.xyz));
		float RdotV = max(0, dot(R, viewDirection.xyz));
		float3 H = normalize(L + V);

		// Calculate the light's specular color
		float4 specular = specular_color * pow(RdotV, shininess);

		// Add specular
		outputColor = saturate(diffuse + specular) * input_color;
		
	}

	return outputColor;

}