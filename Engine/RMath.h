#pragma once
#include <DirectXMath.h>

struct RVector3
{
	float x;
	float y;
	float z;

	RVector3() { x = 0; y = 0; z = 0; };
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

	friend RVector3 operator+(RVector3 lhs, const RVector3& rhs)
	{
		return RVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	void operator+=(RVector3& other)
	{
		x = x + other.x;
		y = y + other.y;
		z = z + other.z;
	}

	friend RVector3 operator-(RVector3 lhs, const RVector3& rhs)
	{
		return RVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	friend const float operator*(RVector3 lhs, const RVector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	friend RVector3 operator*(RVector3 lhs, float muliplier)
	{
		return RVector3(lhs.x * muliplier, lhs.y * muliplier, lhs.z * muliplier);
	}
	friend RVector3 operator/(RVector3 lhs, float divider)
	{
		return RVector3(lhs.x / divider, lhs.y / divider, lhs.z / divider);
	}

	const float size()
	{
		return sqrt(x * x + y * y + z * z);
	}
};
