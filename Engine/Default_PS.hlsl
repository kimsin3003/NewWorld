//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------


Texture2D shaderTexture;
SamplerState SampleType;

struct PS_INPUT
{
	float4 Pos     : SV_POSITION;
	float2 UV			: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, Input.UV);
}
