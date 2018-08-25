#include "Logger.h"
#include <iostream>
#include "ResourceLoader.h"
#include "Share/ResourceInfo.h"
#include "Share/JsonUtility.h"

void main()
{
	ResourceLoader loader;
	ModelInfo info;
	loader.LoadFBX("../Resource/warzombie_f_pedroso.fbx", &info);

	JsonUtility::WriteModelToFIle(&info, "../Resource/zombie.json");

	std::cout << "load complete" << std::endl;
	getchar();
}