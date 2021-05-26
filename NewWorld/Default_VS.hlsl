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
	float3 Tangent	: TANGENT;
	float3 Binormal	: BINORMAL;
};

struct VS_OUTPUT
{
	float4 Pos				: SV_POSITION;
	float2 UV				: TEXCOORD0;
	float3 LightDirection	: TEXCOORD1;
	float3 T				: TANGENT;
	float3 B				: BINORMAL;
	float3 N				: NORMAL;
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

	float3 lightDir = { -1, -1, 1 };
	Output.LightDirection = normalize(lightDir);//(normalize(mul(Input.Pos, worldMatrix) - lightPosition)).xyz;
	//object space=>world space
	float3 worldNormal = mul(Input.Normal, (float3x3)worldMatrix);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.Tangent, (float3x3)worldMatrix);
	Output.T = normalize(worldTangent);

	float3 worldBinormal = mul(Input.Binormal, (float3x3)worldMatrix);
	Output.B = normalize(worldBinormal);
	return Output;
}