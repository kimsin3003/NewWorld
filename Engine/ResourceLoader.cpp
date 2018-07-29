#include "ResourceLoader.h"
#include <string.h>
#include "Material.h"
#include "Logger.h"

FbxManager* g_pFbxSdkManager = nullptr;

bool ResourceLoader::LoadFBX(std::string fbxFileName, std::vector<Mesh*>& outMeshes)
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
			Mesh* mesh = new Mesh();
			LoadVertexInformation(pMesh, mesh->Verticies, mesh->Indicies);
			unsigned int triCount = pMesh->GetPolygonCount(); 
			unsigned int vertexCount = 0; // 정점의 개수 
			for(unsigned int i = 0; i < triCount; ++i) // 삼각형의 개수 
			{ 
				for (unsigned int j = 0; j < 3; ++j) // 삼각형은 세 개의 정점으로 구성 
				{
					int controlPointIndex = pMesh->GetPolygonVertex(i, j); // 제어점 인덱스를 가져온다. 
					LoadUVElement(pMesh, controlPointIndex, pMesh->GetTextureUVIndex(i, j), mesh);
				}
			}

			//LoadUVInformation(pMesh, mesh->Verticies);
			outMeshes.emplace_back(mesh);
		}
	}

	if (outMeshes.size() == 0)
	{
		Logger::Log("loading failed");
		return false;
	}
	return true;
}

void ResourceLoader::LoadVertexInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector, std::vector<unsigned int>& outIndexVector)
{
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	if (!controlPoints)
		return;
	outVertexVector.clear();
	outIndexVector.clear();

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

void ResourceLoader::LoadUVElement(FbxMesh* mesh, int controlPointIndex, int vertexCounter, Mesh* outMesh)
{
	if (mesh->GetElementUVCount() < 1)
		Logger::Log("Invalid ****** Number");
	FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);

	switch (vertexUV->GetMappingMode())
		//switch (vertexTangnet-&gt;GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outMesh->Verticies[controlPointIndex].UV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			outMesh->Verticies[controlPointIndex].UV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(controlPointIndex);
			outMesh->Verticies[controlPointIndex].UV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outMesh->Verticies[controlPointIndex].UV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default:
			Logger::Log("Error: Invalid vertex reference mode!");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outMesh->Verticies[controlPointIndex].UV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(vertexCounter).mData[0]);
			outMesh->Verticies[controlPointIndex].UV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(vertexCounter).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(vertexCounter);
			outMesh->Verticies[controlPointIndex].UV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outMesh->Verticies[controlPointIndex].UV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default:
			Logger::Log("Error: Invalid vertex reference mode!");
		}
		break;
	}
}

void ResourceLoader::LoadUVInformation(FbxMesh* pMesh, std::vector<Vertex>& outVertexVector)
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

					outVertexVector[lPolyVertIndex].UV.x = lUVValue[0];
					outVertexVector[lPolyVertIndex].UV.y = lUVValue[1];
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
						int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

						outVertexVector[lPolyVertIndex].UV.x = lUVValue[0];
						outVertexVector[lPolyVertIndex].UV.y = lUVValue[1];

						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}
