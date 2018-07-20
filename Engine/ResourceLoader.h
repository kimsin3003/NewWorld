#pragma once
#include <vector>
#include <fbxsdk.h>
#include "Material.h"
using namespace fbxsdk;

class ResourceLoader
{
public:
	static bool LoadFBX(std::string fbxFileName, std::vector<struct Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);
};

