#pragma once
#include <DirectXMath.h>

struct RVector3
{
	float x;
	float y;
	float z;

	RVector3() {};
	RVector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};
	RVector3(DirectX::XMFLOAT3 xmlfloat3)
	{
		x = xmlfloat3.x;
		y = xmlfloat3.y;
		z = xmlfloat3.z;
	}

	RVector3 operator-(RVector3& other)
	{
		return RVector3(x - other.x, y - other.y, z - other.z);
	}

	const float operator*(RVector3& other)
	{
		return x * other.x + y * other.y + z * other.z;
	}
	RVector3 operator*(float muliplier)
	{
		RVector3 result = *this;
		result.x *= muliplier;
		result.y *= muliplier;
		result.z *= muliplier;
		return result;
	}
	RVector3 operator/(float divider)
	{
		RVector3 result = *this;
		result.x / divider;
		result.y / divider;
		result.z / divider;
		return result;
	}

	const float size()
	{
		return sqrt(x * x + y * y + z * z);
	}
};
