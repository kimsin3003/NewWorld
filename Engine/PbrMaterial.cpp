#include "PbrMaterial.h"
#include "directxmath.h"


#define _USE_MATH_DEFINES
#include <cmath>

using namespace DirectX;
float GetCos(XMVECTOR v1, XMVECTOR v2)
{
	XMFLOAT3 dotResult;
	XMStoreFloat3(&dotResult, XMVector3Dot(v1, v2));
	return dotResult.x;
}

float PbrMaterial::DiffusePBRT(float in, float out)
{
	return M_1_PI * cos(out); //반구에 대한 cos값의 적분을 구하면 pi다
}

float PbrMaterial::SpecularPBRT(float in, float out)
{
	if (abs(in - out) < Roughness * M_PI / 2)
		return M_1_PI * cos(abs(in - out));
	return 0;
}
