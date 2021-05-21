#pragma once
#include <vector>
#include <fbxsdk.h>
#include <string>
using namespace fbxsdk;

class RResourceLoader
{
public:
	static bool LoadFile(std::string fileName, class RGameObject* gameObj);
};