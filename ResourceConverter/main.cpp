#include "Logger.h"
#include "ResourceConverter.h"
#include "ResouceInfo.h"

void main()
{
	ResourceConverter converter;
	std::vector<MeshInfo*> infos;
	converter.LoadFBX("C:/Users/kimsi/Desktop/NewWorld/Resource/warzombie_f_pedroso.fbx", infos);
}