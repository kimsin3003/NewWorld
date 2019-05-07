#pragma once
#include "RMath.h"

class PbrMaterial
{
public:
	virtual float DiffusePBRT(float in, float out);
	virtual float SpecularPBRT(float in, float out);
	bool IsLight = false;
	RColor Emittance;
	float ReflectRate = 0;
	float Roughness = 1;
	RColor DiffuseConstant = { 0,0,0 };
	RColor SpecularConstant = { 0,0,0 };
	RColor Reflectance; 
	float RefractionRate = 1.0f;
	bool IsTransparent = false;
};

