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
	float3 Normal	: NORMAL0;
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
	Output.Pos = mul(Input.Pos, worldMatrix);
	Output.Pos = mul(Output.Pos, viewMatrix);
	Output.Pos = mul(Output.Pos, projectionMatrix);

	Output.UV = Input.UV;

	Output.Normal = normalize(mul(Input.Normal, (float3x3)worldMatrix));
	float3 lightDir = { 1, 1, 1 };
	Output.LightDirection = lightDir;//(normalize(mul(Input.Pos, worldMatrix) - lightPosition)).xyz;
	return Output;
}