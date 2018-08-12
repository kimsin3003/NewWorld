#include "Logger.h"
#include "ResourceLoader.h"
#include "Share/ResouceInfo.h"

void main()
{
	ResourceLoader converter;
	std::vector<MeshInfo*> infos;
	converter.LoadFBX("C:/Users/kimsi/Desktop/NewWorld/Resource/warzombie_f_pedroso.fbx", infos);
}