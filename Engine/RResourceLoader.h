#pragma once
#include <vector>
#include <fbxsdk.h>
#include <string>
using namespace fbxsdk;

class RResourceLoader
{
public:
	static class RMesh* LoadFile(std::string fileName);
};