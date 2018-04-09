cbuffer cbPerObject : register(b0)
{
	matrix        g_mWorldViewProjection    : packoffset(c0);
	matrix        g_mWorld                : packoffset(c4);
};

struct VS_INPUT
{
	float4 Position    : POSITION;
	float4 Color        : COLOR;
};

struct VS_OUTPUT
{
	float4 Position        : SV_POSITION;
	float4 Color			: COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSShader(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = mul(Input.Position, g_mWorldViewProjection);
	Output.Color = Input.Color;

	return Output;
}