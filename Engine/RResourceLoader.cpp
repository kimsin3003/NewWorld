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
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();
	if (lNormalElement)
	{
		//mapping mode is by control points. The mesh should be smooth and soft.
		//we can get normals by retrieving each control point
		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			//Let's get normals of each vertex, since the mapping mode of normal element is by control point
			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
			{
				int lNormalIndex = 0;
				//reference mode is direct, the normal index is same as vertex index.
				//get normals by the index of control vertex
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					lNormalIndex = lVertexIndex;

				//reference mode is index-to-direct, get normals by the index-to-direct
				if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				//Got normals of each vertex.
				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
				outVertexVector[lVertexIndex].Normal.x = lNormal[0];
				outVertexVector[lVertexIndex].Normal.y = lNormal[2];
				outVertexVector[lVertexIndex].Normal.z = -lNormal[1];
				//add your custom code here, to output normals or get them into a list, such as KArrayTemplate<FbxVector4>
				//. . .
			}//end for lVertexIndex
		}//end eByControlPoint
		 //mapping mode is by polygon-vertex.
		 //we can get normals by retrieving polygon-vertex.
		else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lIndexByPolygonVertex = 0;
			//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
			for (int lPolygonIndex = 0; lPolygonIndex < pMesh->GetPolygonCount(); lPolygonIndex++)
			{
				//get polygon size, you know how many vertices in current polygon.
				int lPolygonSize = pMesh->GetPolygonSize(lPolygonIndex);
				//retrieve each vertex of current polygon.
				for (int i = 0; i < lPolygonSize; i++)
				{
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = lIndexByPolygonVertex;

					//reference mode is index-to-direct, get normals by the index-to-direct
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

					//Got normals of each polygon-vertex.
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolygonIndex, i);
					outVertexVector[lPolyVertIndex].Normal.x = lNormal[0];
					outVertexVector[lPolyVertIndex].Normal.y = lNormal[2];
					outVertexVector[lPolyVertIndex].Normal.z = -lNormal[1];

					lIndexByPolygonVertex++;
				}//end for i //lPolygonSize
			}//end for lPolygonIndex //PolygonCount

		}//end eByPolygonVertex
	}//end if lNormalElement
}
