#pragma once
#include <directxmath.h>

using namespace DirectX;
class RVector3
{
public:
	float x;
	float y;
	float z;

	RVector3() = default;

	RVector3(const RVector3&) = default;
	RVector3& operator=(const RVector3&) = default;
	RVector3(XMFLOAT3 val) : x(val.x), y(val.y), z(val.z) {}
	RVector3(float x, float y, float z) : x(x), y(y), z(z) {}

	RVector3(RVector3&&) = default;
	RVector3& operator=(RVector3&&) = default;
	RVector3 operator*(float val) { return RVector3(x * val, y * val, z * val); }
	RVector3 operator+(RVector3 other) { return RVector3(x + other.x, y + other.y, z + other.z); }
	RVector3 operator-(RVector3 other) { return RVector3(x - other.x, y - other.y, z - other.z); }
	XMFLOAT3 ToXMFloat() { return XMFLOAT3(x,y,z); }

};

