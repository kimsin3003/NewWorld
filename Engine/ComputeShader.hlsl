struct Material
{
	bool IsLight;
	bool IsTransparent;
	bool dummy;
	bool dummy2;
	float Roughness;
	float RefractionRate;
	float ReflectRate;
	float dummy3;
	float4 Emittance;
	float4 DiffuseConstant;
	float4 SpecularConstant;
	float4 Reflectance;
};


struct Object
{
	int PbrColliderType;
	Material Mat;
	float4 Pos;
	float4 Rot;
};

cbuffer Objects : register(b0)
{
	//Buffer<Object> objectList;
};


struct Input 
{

};
//StructuredBuffer<Input> inputArray : register(t0);      // Shader resource binding

struct Output { /* blah blah */ };
//RWStructuredBuffer<Output> outputArray : register(u0);


[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
}