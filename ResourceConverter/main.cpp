#include "Logger.h"
#include "ResourceLoader.h"
#include "Share/ResourceInfo.h"
#include "Share/JsonUtility.h"

void main()
{
	ResourceLoader converter;
	ModelInfo info;
	converter.LoadFBX("C:/Users/kimsin3003/Desktop/NewWorld/Resource/warzombie_f_pedroso.fbx", &info);

	JsonUtility::WriteModelToFIle(&info, "../Resource/zombie.json");
}