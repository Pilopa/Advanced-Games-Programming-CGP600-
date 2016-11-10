cbuffer MatrixBuffer {
	row_major matrix worldViewProjectionMatrix; // 64 bytes
}; // = 64 bytes

float4 main( float3 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL ) : SV_POSITION
{
	return mul(float4(position, 1.0F), worldViewProjectionMatrix);
}