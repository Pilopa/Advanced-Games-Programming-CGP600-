struct VOut {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer CBuffer0
{
    row_major matrix WVPMatrix; // 64 bytes
	float red_fraction; // 4 bytes
	float scale; // 4 bytes
	float2 packing; // 2x4 bytes = 8 bytes
} // = 80 bytes

VOut VShader(float4 position : POSITION, float4 color : COLOR) {
	VOut output;

	float4 pos = mul(position, WVPMatrix);
	output.position = pos;
	output.color = color;

	output.color.r *= red_fraction;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET {
	return color;
}