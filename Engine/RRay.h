#pragma once
#include <directxmath.h>
using namespace DirectX;

class RRay
{
public:
	RRay(int screenX, int screenY, int screenWith, int screenHeight); // for screen to world
	RRay(XMVECTOR origin, XMVECTOR dir) { m_origin = origin; m_dir = dir; }
	XMVECTOR GetOrigin() { return m_origin; }
	XMVECTOR GetDir() { return m_dir; }

private:
	XMVECTOR m_origin;
	XMVECTOR m_dir;
};

