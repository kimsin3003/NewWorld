cbuffer MatrixBuffer 
{ 
	matrix worldMatrix; 
	matrix viewMatrix; 
	matrix projectionMatrix; 
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
VS_OUTPUT VS(VS_INPUT Input)
{
	
	VS_OUTPUT Output;
	Output.Position = Input.Position;
	Output.Color = Input.Color;

	return Output;
}