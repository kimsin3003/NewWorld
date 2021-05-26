//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------


Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
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
	float3 LightDirection	: TEXCOORD1;
	float3 T				: TEXCOORD2;
	float3 B				: TEXCOORD3;
	float3 N				: TEXCOORD4;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : SV_TARGET
{//read normal from tex
	float3 tangentNormal = normalTexture.Sample(SampleType, Input.UV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1); //convert 0~1 to -1~+1.

   //read from vertex shader
	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B),
	  normalize(Input.N)); //transforms world=>tangent space

	TBN = transpose(TBN); //transform tangent space=>world
	float3 worldNormal = mul(TBN, tangentNormal); //note: mat * scalar
	float4 diffuse = saturate(dot(Input.N, -Input.LightDirection));
	float4 albedo = diffuseTexture.Sample(SampleType, Input.UV);
	float4 specular = specularTexture.Sample(SampleType, Input.UV);
	float4 lightColor = { 1,1,1,1 };
	diffuse = lightColor * albedo * diffuse;
	return lightIntensity * (ambientColor + diffuse + specular);
}
