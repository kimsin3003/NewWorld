#pragma once
#include <DirectXMath.h>

struct RColor
{
	float x;
	float y;
	float z;

	RColor() { x = 0; y = 0; z = 0; };
	RColor(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};
	RColor(DirectX::XMFLOAT3 xmlfloat3)
	{
		x = xmlfloat3.x;
		y = xmlfloat3.y;
		z = xmlfloat3.z;
	}

	bool operator==(RColor& other)
	{
		if (x == other.x && y == other.y && z == other.z)
			return true;
		return false;
	}

	friend RColor operator+(RColor lhs, const RColor& rhs)
	{
		return RColor(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	void operator+=(RColor& other)
	{
		x = x + other.x;
		y = y + other.y;
		z = z + other.z;
	}

	friend RColor operator-(RColor lhs, const RColor& rhs)
	{
		return RColor(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	friend const RColor operator*(RColor lhs, const RColor& rhs)
	{
		return RColor(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	friend RColor operator*(RColor lhs, float muliplier)
	{
		return RColor(lhs.x * muliplier, lhs.y * muliplier, lhs.z * muliplier);
	}
	friend RColor operator/(RColor lhs, float divider)
	{
		return RColor(lhs.x / divider, lhs.y / divider, lhs.z / divider);
	}

	const float size()
	{
		return sqrt(x * x + y * y + z * z);
	}

	const RColor normalize()
	{
		return RColor(x / size(), y / size(), z / size());
	}
};
