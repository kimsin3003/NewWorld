#include "Logger.h"
#include <iostream>
#include "ResourceLoader.h"
#include "Share/ResourceInfo.h"
#include "Share/JsonUtility.h"

void main()
{
	ResourceLoader loader;
	ModelInfo info;
	loader.LoadFBX("../Resource/rectangle.FBX", &info);

	JsonUtility::WriteModelToFIle(&info, "../Resource/rectangle.json");

	std::cout << "load complete" << std::endl;
	getchar();
}