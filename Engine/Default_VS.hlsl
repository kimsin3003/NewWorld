cbuffer ConstBuffer 
{ 
	matrix worldMatrix; 
	matrix viewMatrix; 
	matrix projectionMatrix; 
};

struct VS_INPUT
{
	float4 Pos  : POSITION;
	float2 UV	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos     : SV_POSITION;
	float2 UV			: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT Input)
{
	
	VS_OUTPUT Output;
	Output.Pos = Input.Pos;
	Output.Pos = mul(Input.Pos, worldMatrix);
	Output.Pos = mul(Output.Pos, viewMatrix);
	Output.Pos = mul(Output.Pos, projectionMatrix);
	Output.UV = Input.UV;

	return Output;
}