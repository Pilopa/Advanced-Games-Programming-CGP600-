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
	float4 viewPosition;
	float4 lightPosition;
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
	float lightDistance;
	float4 lightDirection;
	float calculatedLightIntensity;
	float4 outputColor;
	float4 h;
	float4 diffuse;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	input_color = colorTexture.Sample(SampleTypePoint, input.tex);

	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	input_normal = normalTexture.Sample(SampleTypePoint, input.tex);

	// Sample the positions from the position render texture using the point sampler at this texture coordinate location.
	input_position = positionTexture.Sample(SampleTypePoint, input.tex);

	// Set default output color
	outputColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// Calculate light vector
	float3 L = (lightPosition - input_position).xyz;
	float distance = length(L);
	L = L / distance;

	float NdotL = saturate(dot(input_normal.xyz, L));

	if (!(input_normal.r == 0.0f && input_normal.g == 0.0f
		&& input_normal.b == 0.0f && input_normal.a == 0.0f)
		&& NdotL > 0.0f) {

		float4 specular_color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		// Calculate attenuation
		float attenuation = 1.0f / (1.0f + 0.2f * distance + 0.1f * distance * distance);

		// Calculate view vector
		float3 V = normalize(viewPosition - input_position).xyz;

		// Calculate diffuse
		diffuse = saturate(lightColor * NdotL) * attenuation;

		// Calculate reflection
		float3 R = normalize(reflect(-L, input_normal.xyz));
		float RdotV = max(0, dot(R, V));
		float3 H = normalize(L + V);

		// Calculate the light's specular color
		float4 specular = specular_color * pow(RdotV, shininess) * attenuation;

		// Combine to output color
		outputColor = (diffuse + specular) * input_color;

	}

	return outputColor;

}