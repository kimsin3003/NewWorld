cbuffer cbPerObject : register(b0)
{
	matrix        g_mWorldViewProjection    : packoffset(c0);
	matrix        g_mWorld                : packoffset(c4);
};

struct VS_INPUT
{
	float4 Position    : POSITION;
	float4 Color        : COLOR;
	float2 vTexcoord    : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position        : NORMAL;
	float4 Color			: TEXCOORD0;
	float4 vPosition    : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSShader(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.vPosition = mul(Input.vPosition, g_mWorldViewProjection);
	Output.vNormal = mul(Input.vNormal, (float3x3)g_mWorld);
	Output.vTexcoord = Input.vTexcoord;

	return Output;
}