cbuffer VSConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 lightPosition;
};

struct VS_INPUT
{
	float4 Pos		: POSITION;
	float2 UV		: TEXCOORD0;
	float3 Normal	: NORMAL;
};

struct VS_OUTPUT
{
	float4 Pos				: SV_POSITION;
	float2 UV				: TEXCOORD0;
	float3 Normal			: NORMAL;
	float3 LightDirection	: TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.Pos = Input.Pos;
	Output.UV = Input.UV;
	return Output;
}