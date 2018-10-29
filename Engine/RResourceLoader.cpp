#include "RResourceLoader.h"
#include <iostream>
#include "Logger.h"
#include "RMesh.h"
#include "RGameObject.h"
#include "JsonUtility.h"

static FbxManager* g_pFbxSdkManager = nullptr;


bool RResourceLoader::LoadFBX(RGameObject* outGameObject, std::string fbxFileName)
{
	std::string jsonFileName = "../Resource/" + fbxFileName + ".json";
	std::string fbxFileDir = "../Resource/" + fbxFileName;
	if (JsonUtility::ReadModelFromFile(outGameObject, jsonFileName))
		return true;

	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(fbxFileDir.c_str(), -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess)
	{
		FbxString error = pImporter->GetStatus().GetErrorString();
		std::cout << "importer initialize failed :" << error.Buffer() << std::endl;
		Logger::ShowAlert(fbxFileDir + " deosn't exist");
		return false;
	}
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		FbxString error = pImporter->GetStatus().GetErrorString();
		std::cout << "importer import failed :" << error.Buffer() << std::endl;
		return false;
	}

	pImporter->Destroy();

	FbxGeometryConverter lGeomConverter(g_pFbxSdkManager);
	if (!lGeomConverter.Triangulate(pFbxScene, true))
		return false;
	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	FbxDouble3 lTranslation = pFbxRootNode->LclTranslation.Get();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			FbxMesh* pMesh = pFbxChildNode->GetMesh();
			if (!pMesh)
				continue;

			RMesh* mesh = new RMesh();
			LoadVertexInformation(pMesh, mesh->Verticies, mesh->Indicies);
			LoadUVInformation(pMesh, mesh->Verticies);
			LoadNormalInformation(pMesh, mesh->Verticies);
			outGameObject->Meshes.emplace_back(mesh);
		}
	}

	if (outGameObject->Meshes.size() == 0)
	{
		std::cout << "loading failed" << std::endl;
		return false;
	}


	JsonUtility::WriteModelToFIle(outGameObject, jsonFileName);
	return true;
}

void RResourceLoader::LoadVertexInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector, std::vector<unsigned int>& outIndexVector)
{
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	if (!controlPoints)
		return;
	outVertexVector.clear();
	outIndexVector.clear();

	for (int j = 0; j < pMesh->GetControlPointsCount(); j++)
	{
		RVertex vertex;
		vertex.Pos.x = (float)controlPoints[j].mData[0];
		vertex.Pos.y = (float)controlPoints[j].mData[2];
		vertex.Pos.z = -(float)controlPoints[j].mData[1];
		outVertexVector.emplace_back(vertex);
	}

	int* iControlPointIndicies = pMesh->GetPolygonVertices();
	for (int j = 0; j < pMesh->GetPolygonVertexCount(); j++)
	{
		outIndexVector.push_back(iControlPointIndicies[j]);
	}
}

void RResourceLoader::LoadUVInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		for (int i = 0; i < pMesh->GetPolygonCount(); i++)
		{
			int lPolygonsize = pMesh->GetPolygonSize(i);

			for (int pol = 0; pol < lPolygonsize; pol++)
			{
				FbxString name = lUVSetNameList.GetStringAt(lUVSetIndex);
				FbxVector2 texCoord;
				bool unmapped = false;
				if (!pMesh->GetPolygonVertexUV(i, pol, name, texCoord, unmapped))
					return;

				int lPolyVertIndex = pMesh->GetPolygonVertex(i, pol);
				outVertexVector[lPolyVertIndex].UV.x = texCoord.mData[0];
				outVertexVector[lPolyVertIndex].UV.y = 1 - texCoord.mData[1];
			}
		}
	}
}

void RResourceLoader::LoadNormalInformation(FbxMesh* pMesh, std::vector<RVertex>& outVertexVector)
{
	for (int i = 0; i < pMesh->GetPolygonCount(); i++)
	{
		int lPolygonsize = pMesh->GetPolygonSize(i);

		for (int pol = 0; pol < lPolygonsize; pol++)
		{
			FbxVector4 normal;
			if (pMesh->GetPolygonVertexNormal(i, pol, normal))
				return;

			int lPolyVertIndex = pMesh->GetPolygonVertex(i, pol);
			outVertexVector[lPolyVertIndex].Normal.x = normal[0];
			outVertexVector[lPolyVertIndex].Normal.y = normal[2];
			outVertexVector[lPolyVertIndex].Normal.z = -normal[1];
		}
	}
}
