//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------


Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PSConstBuffer
{
	float lightIntensity;
	float3 dummy;
	float4 ambientColor;
};

struct PS_INPUT
{
	float4 Pos				: SV_POSITION;
	float2 UV				: TEXCOORD0;
	float3 Normal			: NORMAL;
	float3 LightDirection	: TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, Input.UV);
	return textureColor;
}
