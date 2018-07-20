
#include "ResourceLoader.h"
#include <string.h>
#include "Mesh.h"
#include "Logger.h"

FbxManager* g_pFbxSdkManager = nullptr;

static void LoadVertexInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector);
static void LoadUVInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector);

bool ResourceLoader::LoadFBX(std::string fbxFileName, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector)
{
	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(fbxFileName.c_str(), -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess)
	{
		Logger::Log("importer initialize failed");
		return false;
	}
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		Logger::Log("importer import failed");
		return false;
	}

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			FbxMesh* pMesh = pFbxChildNode->GetMesh();
			if (!pMesh)
				continue;

			LoadVertexInformation(pMesh, outVertexVector, outIndexVector);
			//LoadUVInformation(pMesh, outVertexVector);
		}
	}

	if (outVertexVector.size() == 0 || outIndexVector.size() == 0)
	{
		Logger::Log("loading failed");
		return false;
	}
	return true;
}

static void LoadVertexInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector)
{
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	if (!controlPoints)
		return;

	for (int j = 0; j < pMesh->GetControlPointsCount(); j++)
	{
		Vertex vertex;
		vertex.Pos.x = (float)controlPoints[j].mData[0];
		vertex.Pos.y = (float)controlPoints[j].mData[1];
		vertex.Pos.z = (float)controlPoints[j].mData[2];
		outVertexVector.emplace_back(vertex);
	}

	int* iControlPointIndicies = pMesh->GetPolygonVertices();
	for (int j = 0; j < pMesh->GetPolygonVertexCount(); j++)
	{
		outIndexVector.push_back(iControlPointIndicies[j]);
	}
}

static void LoadUVInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

					outVertexVector[lPolyVertIndex].UV.x = lUVValue.mData[0];
					outVertexVector[lPolyVertIndex].UV.y = lUVValue.mData[1];
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						outVertexVector[lPolyIndexCounter].UV.x = lUVValue.mData[0];
						outVertexVector[lPolyIndexCounter].UV.y = lUVValue.mData[1];

						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}

// static void LoadTextureInformation(class FbxMesh* pMesh, TextureInfo* diffuse)
// {
// 	int lMaterialIndex;
// 	FbxProperty lProperty;
// 	if (pMesh->GetNode() == NULL)
// 		return;
// 	int lNbMat = pMesh->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
// 	for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++) {
// 		FbxSurfaceMaterial *lMaterial = pMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
// 		bool lDisplayHeader = true;
// 		if (lMaterial) {
// 			int lTextureIndex;
// 			FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
// 			{
// 				lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
// 				int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
// 				for (int j = 0; j < lTextureCount; ++j)
// 				{
// 					FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>(j);
// 					if (lTexture)
// 					{
// 						std::string filename = lTexture->GetFileName();
// 						std::wstring temp(filename.length(), L' ');
// 						std::copy(filename.begin(), filename.end(), temp.begin());
// 						diffuse->filename = temp;
// 						diffuse->type = TextureInfo::DIFFUSE;
// 					}
// 				}
// 			}
// 		}
// 	}
// }
