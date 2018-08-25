//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------


Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PSConstBuffer
{
	float4 lightPosition;
	float lightIntensity;
	float3 dummy;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
	float4 Normal	: NORMAL0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : SV_TARGET
{
	float4 lightDirection = normalize(Input.Pos - lightPosition);
	float4 textureColor = shaderTexture.Sample(SampleType, Input.UV);
	float4 diffuse = max(lightIntensity * dot(Input.Normal, lightDirection), 0);
	return textureColor * diffuse;
}
