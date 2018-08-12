#include "JsonUtility.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

void JsonUtility::WriteFBXToFIle(std::vector<MeshInfo*> infos, std::string filename)
{
	// 1. Parse a JSON string into DOM.
	Document d;
	d.SetObject();

	//Value& s;
}
