#include "Logger.h"
#include "ResourceConverter.h"

void main()
{
	Logger::Initialize("RCLog.txt");
	ResourceConverter converter;
	std::vector<MeshInfo*> infos;
	converter.LoadFBX("C:/Users/kimsi/Desktop/NewWorld/Resource/warzombie_f_pedroso.fbx", infos);
}