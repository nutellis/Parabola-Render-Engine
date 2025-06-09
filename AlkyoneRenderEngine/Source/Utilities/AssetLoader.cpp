#include <Utilities/AssetLoader.h>

#include <Managers/LogManager.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Components/Material.h>
#include <Components/Texture.h>


#include <Components/StaticMesh.h>
#include <Components/Colliders/AxisAlignedBoundingBox.h>
#include <algorithm>
#include <Utilities/FileUtilities.h>
#include <GenericMath.h>



template<> GAssetLoader* SingletonManagerBase<GAssetLoader>::instance = 0;
GAssetLoader & GAssetLoader::getInstance()
{
	return (*instance);
}

GAssetLoader * GAssetLoader::getInstancePtr()
{
	if (instance == nullptr)
	{
		instance = new GAssetLoader();
	}
	return instance;
}


GAssetLoader::GAssetLoader()
{
}

GAssetLoader::~GAssetLoader()
{
}


Asset * GAssetLoader::LoadAsset(const char * filepath)
{
	Asset * Result = new Asset();

	LOG(DEBUG, "IMPORTING MESH...");
	if (FileUtilities::FileExtension(filepath) == ".obj")
	{
		
		ObjLoader Loader;// = ObjLoader();

		Result = Loader.Read(filepath);
	}
	else if (FileUtilities::FileExtension(filepath) ==(".fbx"))
	{
		FbxLoader Loader;// = FbxLoader();

		Result = Loader.Read(filepath);

	}
	else
	{
		//error mate. File not supported. oups
		LOG(ERROR, "File format not supported");
		exit(-1);
	}



	LOG(DEBUG, "MESH IMPORTED");
	return Result;
}

void GAssetLoader::Init()
{
	LOG(INFO, "ASSETLOADER INITIATED\n");
}

void GAssetLoader::Terminate()
{
	LOG(INFO, "ASSETLOADER Terminated\n");
}


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------

FbxLoader::FbxLoader()
{
	//AssetArchive = new Archive();
}
FbxLoader::~FbxLoader()
{
	//delete AssetArchive;
}


Asset * FbxLoader::Read(const char* Filepath)
{
	bool isRead = true;
	FbxManager *lManager = FbxManager::Create();
	Settings(lManager);

	FbxImporter *lImporter = FbxImporter::Create(lManager, "");
	if (!lImporter->Initialize(Filepath, -1, lManager->GetIOSettings())) {

		LOG(ERROR, "Call to FbxImporter::Initialize() failed.");
		LOG(ERROR, "Error returned : %s", lImporter->GetStatus().GetErrorString());

		isRead = false;
	}
	FbxScene * aScene = FbxScene::Create(lManager, "Asset");
	lImporter->Import(aScene);

	lImporter->Destroy();

	//std::cout<<aScene->GetRootNode()->GetChildCount()<<std::endl;

	//std::cout << "\n\nFetching Mesh\n" << std::endl;
	//path = splitPath(Filepath); 

	return Build(aScene->GetRootNode());

}

FbxIOSettings* FbxLoader::Settings(FbxManager *manager)
{
	FbxIOSettings *settings = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);

	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
	(*(manager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);


	return settings;
}

Vector3f FbxLoader::Convert(FbxVector4 & ToConvert)
{
	return Vector3f(ToConvert.mData[0], ToConvert.mData[1], ToConvert.mData[2]);
}

Asset * FbxLoader::Build(FbxNode *node)
{
	bool isFetched = false;
	Asset * format = new Asset();

	FbxNodeAttribute *attribute = node->GetNodeAttribute();
	if (attribute)
	{
		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh *Mesh = node->GetMesh();


			//Testing materials
			//format->Materials = FetchMaterial(node);

			int32 PolygonCount = Mesh->GetPolygonCount();


#if 0
			std::cout << "GetPolygonCount (Triangle Count): " << mesh->GetPolygonCount() << "\n\n";


			std::cout << "GetPolygonSize (3 for triangle, 4 for quads etc): " << mesh->GetPolygonSize(5) << "\n\n";


			int countind = mesh->GetPolygonVertexCount();
			std::cout << "GetPolygonVertexCount (Indicies Count): " << countind << "\n\n";

			std::cout << "UV COUNT: " << mesh->GetTextureUVCount() << "\n\n";

			std::cout << "UV Layer Count: " << mesh->GetUVLayerCount() << "\n\n";

			std::cout << "UV Element Count: " << mesh->GetElementUVCount() << "\n\n";

			FbxStringList uvnames;

			mesh->GetUVSetNames(uvnames);

			for (i = 0; i < uvnames.GetCount(); i++)
			{
				std::cout << "UV Set name: " << uvnames[i] << "\n\n";

			}
			int LayerCount = mesh->GetLayerCount();
			std::cout << "Layer Count: " << LayerCount << "\n\n";
#endif

			FbxLayer* BaseLayer = Mesh->GetLayer(0);

			FbxLayerElementMaterial* LayerElementMaterial = BaseLayer->GetMaterials();
			FbxLayerElement::EReferenceMode MaterialReferenceMode(FbxLayerElement::eIndexToDirect);
			FbxLayerElement::EMappingMode MaterialMappingMode(FbxLayerElement::eByPolygon);
			if (LayerElementMaterial)
			{
				hasMaterials = true;
				MaterialReferenceMode = LayerElementMaterial->GetReferenceMode();
				MaterialMappingMode = LayerElementMaterial->GetMappingMode();
			}
			FbxLayerElementNormal* LayerElementNormal = BaseLayer->GetNormals();

			FbxLayerElement::EReferenceMode NormalReferenceMode(FbxLayerElement::eDirect);
			FbxLayerElement::EMappingMode NormalMappingMode(FbxLayerElement::eByControlPoint);
			
			if (LayerElementNormal)
			{
				hasNormals = true;
				NormalReferenceMode = LayerElementNormal->GetReferenceMode();
				NormalMappingMode = LayerElementNormal->GetMappingMode();
			}

			//Fetch Position
			int32 VertexCount = Mesh->GetControlPointsCount();

			RawPositions = TArray<Vector3f>(VertexCount);

			Vector3f VertexPosition(0.0f);

			for (int32 VertexIndex = 0; VertexIndex < VertexCount; ++VertexIndex)
			{
				FbxVector4 FbxPosition = Mesh->GetControlPoints()[VertexIndex];
				VertexPosition.X = FbxPosition.mData[0];
				VertexPosition.Y = FbxPosition.mData[1];
				VertexPosition.Z = FbxPosition.mData[2];

				RawPositions.PushBack(VertexPosition);
			}

			int32 RealFbxVertexIndex = 0, ControlPointIndex = 0;

			/*result.X = static_cast<float>(LayerElementMaterial->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			result.Y = static_cast<float>(gVertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);*/

			const int* polygonIndicesArray = Mesh->GetPolygonVertices();

			//number of polugons(triangles for us) For each polygon
			for (int32 PolygonIndex = 0; PolygonIndex < PolygonCount; PolygonIndex++)
			{

				int32 PolygonVertexCount = Mesh->GetPolygonSize(PolygonIndex);

				Vector3f position(0.0f);
				Vector3f normal(0.0f);
				Vector2f uv(0.0f);

				//MaterialIndexMapping[LayerElementMaterial->GetIndexArray()[PolygonIndex]] = TArray<uint32>();

				//Vector3f binormal(0.0f);
				//Vector3f tangent(0.0f);
				//indexes(corners) per polygon (triangles for us, actually vertices). For each vertex per polygon
				for (int32 CornerIndex = 0; CornerIndex < PolygonVertexCount; CornerIndex++, RealFbxVertexIndex++)
				{
					ControlPointIndex = Mesh->GetPolygonVertex(PolygonIndex, CornerIndex);

					//Indices.PushBack(polygonIndicesArray[Mesh->GetPolygonVertexIndex(PolygonIndex) + CornerIndex]);

					Positions.PushBack(RawPositions[ControlPointIndex]);

					if (LayerElementMaterial) {
						switch (MaterialMappingMode) {
						case FbxLayerElement::eAllSame: {
							//MaterialIndexMapping[LayerElementMaterial->GetIndexArray().GetAt(0)].PushBack(ControlPointIndex);
						}
						break;
						case FbxLayerElement::eByPolygon: {
							//MaterialIndexMapping[LayerElementMaterial->GetIndexArray().GetAt(PolygonIndex)].PushBack(ControlPointIndex);
							MaterialPerPolygonIndices.PushBack(LayerElementMaterial->GetIndexArray()[PolygonIndex]);
						}
						break;
						}
					}
					

					//Fetching Normals
					if (hasNormals)
					{

						int32 NormalMapIndex, NormalValueIndex;

						switch (NormalMappingMode)
						{
						case FbxLayerElement::EMappingMode::eByControlPoint: {
							
							//one normal per vertex. Get the actual normal from the normal array!
							NormalMapIndex = ControlPointIndex;
							break;
						}
						case FbxLayerElement::EMappingMode::eByPolygonVertex: {

							// this means one normal per vertex per polygon (smooth shading)
							//if there are more than one normal here, we need to interpolate between those OR use only the first normal





							// how are we going to know that is an other question
							NormalMapIndex = RealFbxVertexIndex;
							break;
						}
						case FbxLayerElement::EMappingMode::eByPolygon: {
							// this means one normal per polygon (flat shading)

							// in this case we dont use indexing. We use straight up the raw data.

							NormalMapIndex = RealFbxVertexIndex;
							break;
						}
						// eNone, eByEdge and eAllSame are not applicable
						default: break;
						}

						if (NormalReferenceMode == FbxLayerElement::eDirect)
						{
							NormalValueIndex = NormalMapIndex;
						}
						else
						{
							NormalValueIndex = LayerElementNormal->GetIndexArray().GetAt(NormalMapIndex);
						}

						FbxVector4 FbxNormal = LayerElementNormal->GetDirectArray().GetAt(NormalValueIndex);

						Normals.PushBack(Vector3f(FbxNormal.mData[0], FbxNormal.mData[1], FbxNormal.mData[2]));
					}

					UVs.PushBack(FetchUVs(Mesh, ControlPointIndex, Mesh->GetTextureUVIndex(PolygonIndex, CornerIndex), 0));

					//uv = FetchUVs(Mesh, ControlPointIndex, Mesh->GetTextureUVIndex(PolygonIndex, CornerIndex), 0);


					//InsertVertex(position, normal, uv);
				//	std::cout << x++ << "\n";

				}
			}
  			InsertVertex(hasUVs, hasNormals);
			//actually use this data


			format->Vertices = Vertices;
			format->Indices = Indices;
			format->MaterialIndexMapping = MaterialPerPolygonIndices;
			//format->MaterialIndexMapping = MaterialIndexMapping;
			//format->BoundingBox.SetExtents(Min, Max);

			//format.material = mat;

			/*std::cout << MaterialIndexMapping[1].Size() << "\n";
			for (auto i = 0; i < MaterialIndexMapping[1].Size(); i++) {
				std::cout << MaterialIndexMapping[1][i] << "\t";
			}*/

			//Serialize(*AssetArchive);
			//AssetArchive->Finalize(path);

			isFetched = true;

		}
	}
	if (!isFetched) {
		const int childCount = node->GetChildCount();
		for (int i = 0; i < childCount; i++) {
			format = Build(node->GetChild(i));
		}
	}

	return format;
}

void FbxLoader::FetchPosition(FbxMesh * mesh)
{

	int32 VertexCount = mesh->GetControlPointsCount();

	Vector3f VertexPosition(0.0f);

	for (uint32 VertexIndex = 0; VertexIndex <= VertexCount; ++VertexIndex)
	{
		FbxVector4 FbxPosition = mesh->GetControlPoints()[VertexIndex];
		VertexPosition.X = FbxPosition.mData[0];
		VertexPosition.Y = FbxPosition.mData[1];
		VertexPosition.Z = FbxPosition.mData[2];

		RawPositions.PushBack(VertexPosition);
	}
}

Vector3f FbxLoader::FetchNormal(FbxMesh * mesh, int gVertexIndex, int gVertexCounter)
{
	if (mesh->GetElementNormalCount() < 1)
	{
		LOG(INFO, "No normals!");
		hasNormals = false;
	}
	else {
		hasNormals = true;
	}

	FbxGeometryElementNormal *normalData = mesh->GetElementNormal(0);
	Vector3f normal(0.0f);

	switch (normalData->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (normalData->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{

			normal.X = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexIndex).mData[0]);
			normal.Y = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexIndex).mData[1]);
			normal.Z = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{

			int index = normalData->GetIndexArray().GetAt(gVertexIndex);
			normal.X = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[0]);
			normal.Y = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[1]);
			normal.Z = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			LOG(ERROR, "Error: Invalid VertexFormat reference mode!");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (normalData->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			//std::cout << "edw eisai" << std::endl;
			normal.X = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexCounter).mData[0]);
			normal.Y = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexCounter).mData[1]);
			normal.Z = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{

			int index = normalData->GetIndexArray().GetAt(gVertexCounter);
			normal.X = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[0]);
			normal.Y = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[1]);
			normal.Z = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			LOG(ERROR, "Error: Invalid VertexFormat reference mode!");
		}
		break;

	case FbxLayerElement::EMappingMode::eByPolygon: {
		// this means one normal per polygon (flat shading)

		// in this case we dont use indexing. We use straight up the raw data.
		switch (normalData->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			//std::cout << "edw eisai" << std::endl;
			normal.X = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexCounter).mData[0]);
			normal.Y = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexCounter).mData[1]);
			normal.Z = static_cast<float>(normalData->GetDirectArray().GetAt(gVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{

			int index = normalData->GetIndexArray().GetAt(gVertexCounter);
			normal.X = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[0]);
			normal.Y = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[1]);
			normal.Z = static_cast<float>(normalData->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			LOG(ERROR, "Error: Invalid VertexFormat reference mode!");
		}
		break;
	}
	}

	return normal;

}

Vector2f FbxLoader::FetchUVs(FbxMesh * mesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer)
{

	if (inUVLayer >= 2 || mesh->GetElementUVCount() <= inUVLayer)
	{
		LOG(INFO, "Invalid UV Layer Number");
		hasUVs = false;
	}
	else {
		hasUVs = true;
	}

	const FbxGeometryElementUV* gVertexUV = mesh->GetElementUV(inUVLayer);

	Vector2f result;
	switch (gVertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (gVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.X = static_cast<float>(gVertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			result.Y = static_cast<float>(gVertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = gVertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			result.X = static_cast<float>(gVertexUV->GetDirectArray().GetAt(index).mData[0]);
			result.Y = static_cast<float>(gVertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default:
			LOG(ERROR, "Error: Invalid VertexFormat reference mode!");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (gVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			result.X = static_cast<float>(gVertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			result.Y = static_cast<float>(gVertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;
		default:
			LOG(ERROR, "Error: Invalid VertexFormat reference mode!");
		}
		break;
	}

	return result;
}
//
//Vector3f FbxLoader::FetchBinormal(FbxMesh* mesh, int controlPointIndex, int gVertexCounter)
//{
//	if (mesh->GetElementBinormalCount() < 1)
//		std::cout << "Invalid Binormal Number" << std::endl;
//
//	FbxGeometryElementBinormal* gVertexBinormal = mesh->GetElementBinormal(0);
//	Vector3f result;
//	switch (gVertexBinormal->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (gVertexBinormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			result.X = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//			result.Y = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//			result.Z = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = gVertexBinormal->GetIndexArray().GetAt(controlPointIndex);
//			result.X = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(index).mData[0]);
//			result.Y = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(index).mData[1]);
//			result.Z = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//		default:
//			std::cout << "Error: Invalid VertexFormat reference mode!" << std::endl;
//		}
//		break;
//
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (gVertexBinormal->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			result.X = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(gVertexCounter).mData[0]);
//			result.Y = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(gVertexCounter).mData[1]);
//			result.Z = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(gVertexCounter).mData[2]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = gVertexBinormal->GetIndexArray().GetAt(gVertexCounter);
//			result.X = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(index).mData[0]);
//			result.Y = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(index).mData[1]);
//			result.Z = static_cast<float>(gVertexBinormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//		default:
//			std::cout << "Error: Invalid VertexFormat reference mode!" << std::endl;
//		}
//		break;
//	}
//
//	return result;
//}
////
//Vector3f FbxLoader::FetchTangent(FbxMesh* mesh, int controlPointIndex, int gVertexCounter)
//{
//	if (mesh->GetElementTangentCount() < 1)
//		std::cout << "Invalid Tangent Number" << std::endl;
//
//	FbxGeometryElementTangent* gVertexTangent = mesh->GetElementTangent(0);
//	Vector3f result;
//	switch (gVertexTangent->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//		switch (gVertexTangent->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			result.X = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//			result.Y = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//			result.Z = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = gVertexTangent->GetIndexArray().GetAt(controlPointIndex);
//			result.X = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(index).mData[0]);
//			result.Y = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(index).mData[1]);
//			result.Z = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//		default:
//			std::cout << "Error: Invalid VertexFormat reference mode!" << std::endl;
//		}
//		break;
//
//	case FbxGeometryElement::eByPolygonVertex:
//		switch (gVertexTangent->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			result.X = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(gVertexCounter).mData[0]);
//			result.Y = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(gVertexCounter).mData[1]);
//			result.Z = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(gVertexCounter).mData[2]);
//		}
//		break;
//
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = gVertexTangent->GetIndexArray().GetAt(gVertexCounter);
//			result.X = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(index).mData[0]);
//			result.Y = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(index).mData[1]);
//			result.Z = static_cast<float>(gVertexTangent->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//		default:
//			std::cout << "Error: Invalid VertexFormat reference mode!" << std::endl;
//		}
//		break;
//	}
//	return result;
//}

TArray<PMaterial> FbxLoader::FetchMaterial(FbxNode *node)
{
	TArray<PMaterial> Materials = TArray<PMaterial>();
	int MaterialCount = node->GetMaterialCount();
	for (int i = 0; i < MaterialCount; i++)
	{
		PMaterial NewMaterial = PMaterial();
		FbxSurfaceMaterial* FbxMaterial = node->GetMaterial(i);

		NewMaterial.MaterialID = FbxMaterial->GetUniqueID();

		NewMaterial.Emissive = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

		/*NewMaterial.Ambient = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);*/

		NewMaterial.Diffuse = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);

		NewMaterial.Specular = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

		FbxProperty lShininessProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
		if (lShininessProperty.IsValid())
		{
			double lShininess = lShininessProperty.Get<FbxDouble>();
			NewMaterial.Roughness.Colours = static_cast<float>(lShininess);
		}

		Materials.PushBack(NewMaterial);
	}
	return Materials;
}
// 

PChannel* FbxLoader::GetMaterialProperty(const FbxSurfaceMaterial* pMaterial,
	const char* pPropertyName,
	const char* pFactorPropertyName)
{
	PChannel* lResult = new PChannel();
	const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
	const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
	if (lProperty.IsValid() && lFactorProperty.IsValid())
	{
		lResult->Name = pPropertyName;
		FbxDouble3 lColour = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1)
		{
			lColour[0] *= lFactor;
			lColour[1] *= lFactor;
			lColour[2] *= lFactor;
		}
		lResult->Colours = Vector4f((float)lColour[0], (float)lColour[1], (float)lColour[2], 1.0f);
	}

	if (lProperty.IsValid())
	{
		//const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		//if (lTextureCount)
		//{
		//	const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>();
		//	lResult->ChannelTexture->Generate(lTexture->GetFileName()); // LoadTexture
		//	lResult->HasTexture = true;
		//}

	}

	return lResult;
}


void BaseLoader::InsertVertex(bool hasUVs, bool hasNormals)
{
	VertexFormat Vertex;

	Vector3f Zero = Vector3f::ZERO;

	Max = Vector3f::ZERO;
	Min = Vector3f::ZERO;

	for (int i = 0; i < Positions.Size(); i++) {
		if (hasUVs && hasNormals)
		{
			Vertex = { Positions[i], Normals[i], UVs[i] };
		}
		else if (hasUVs)
		{
			Vertex = { Positions[i], Zero, UVs[i] };
		}
		else if (hasNormals)
		{
			Vertex = { Positions[i], Normals[i], Zero };
		}
		else
		{
			Vertex = { Positions[i], Zero, Zero };
		}

		auto lookup = IndexMapping.find(Vertex);
		if (lookup != IndexMapping.end())
		{

			Indices.PushBack(lookup->second);
		}
		else
		{

			uint32 index = static_cast<uint32>(Vertices.Size());
			IndexMapping[Vertex] = index;
			Indices.PushBack(index);
			Vertices.PushBack(Vertex);
		}

		/*if (doIndices) {
			auto lookup = IndexMapping.find(Vertex);
			if (lookup != IndexMapping.end())
			{

				Indices.PushBack(lookup->second);
			}
			else
			{

				uint32 index = static_cast<uint32>(Vertices.Size());
				IndexMapping[Vertex] = index;
				Indices.PushBack(index);
				Vertices.PushBack(Vertex);
			}
		}*/

		if (Positions[i] > Max)
		{
			Max = Positions[i];
		}
		else if (Positions[i] < Min)
		{
			Min = Positions[i];
		}
	}
}

//
////
////void FbxLoader::Serialize(Archive &Ar)
////{
////	size_t gVertexSize = format.Vertex.size();
////	size_t indexSize = format.indices.size();
////	//size_t MaterialSize = sizeof(format.material);
////
////	////Vertex
////	std::cout << "Writing Vertex data" << std::endl;
////	Ar.ArchiveWrite(&gVertexSize, sizeof(size_t));
////
////	Ar.ArchiveWrite(&format.Vertex[0], sizeof(VertexFormat)*gVertexSize);
////
////	////Index
////	std::cout << "Writing Index data" << std::endl;
////	Ar.ArchiveWrite(&indexSize, sizeof(size_t));
////
////	Ar.ArchiveWrite(&format.indices[0], sizeof(unsigned int)*indexSize);
////
////	//Material
////	std::cout << "Writing Material data" << std::endl;
////	//format.material->Serialize(Ar);
////}
//
///*
//Asset * FbxLoader::Deserialize(Archive &Ar) {
//
//	Ar.LoadToArchive(path);
//
//	unsigned int i = 0;
//
//	size_t gVertexSize = 0, indexSize = 0;
//
//	Asset *data = new Asset;
//
//	std::ifstream in;
//	in.open(path, std::ios::binary);
//
//	//Vertex
//
//	Ar.ArchiveRead(&gVertexSize, sizeof(size_t));
//	
//	//in.read((char*)&gVertexSize, sizeof(size_t));
//	data->Vertex = std::vector<VertexFormat>(gVertexSize);
//	//
//	//for (i = 0; i < gVertexSize; i++) {
//	//	in.read((char*)&data->VertexFormat[i], sizeof(VertexFormat));
//	//}
//	Ar.ArchiveRead(&data->Vertex[0], sizeof(VertexFormat)*gVertexSize);
//	std::cout << data->Vertex[0].Position.X << std::endl;
//	
//	////Index
//	Ar.ArchiveRead(&indexSize, sizeof(size_t));
//	//in.read((char*)&indexSize, sizeof(size_t));
//	data->indices = std::vector<size_t>(indexSize);
//	
//	//for (i = 0; i < indexSize; i++) {
//	//	in.read((char*)&data->indices[i], sizeof(unsigned int));
//	//}
//	Ar.ArchiveRead(&data->indices[0], sizeof(unsigned int)*indexSize);
//	std::cout << data->indices[2]<< std::endl;
//	//in.close();
//	
//	//data->material = new PMaterial();
//	//data->material->Deserialize(Ar);
//
//	return data;
//	
//
//}
//*/
//
//
//
////-----------------------------------------------------------------------
////								MATERIAL
////-----------------------------------------------------------------------
///*
//PMaterial::PMaterial() : mShinness(0)
//{
//	mEmissive = new Channel();
//	mAmbient = new Channel();
//	mDiffuse = new Channel();
//	mSpecular = new Channel();
//}
//
//PMaterial::~PMaterial()
//{
//	delete mEmissive;
//	delete mAmbient;
//	delete mDiffuse;
//	delete mSpecular;
//}
//
//void * AssetLoader::InitializeMaterial(const FbxSurfaceMaterial * pMaterial)
//{
//	
//}
//


///*
//// "C:/Users/Nutellis/Documents/Visual Studio 2015/Projects/OpenGLTutorial/Engine-core/Resources/container2.png"
////RTexture *PMaterial::LoadTexture(const char * filename) //, GLenum image_format, int internal_format, int level, int border
////{
////	RTexture *tempTexture = new RTexture();
////	bool TextureLoaded = true;
////	//image format
////	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
////	//pointer to the image, once loaded
////	FIBITMAP *dib(0);
////
////	//check the file signature and deduce its format
////	fif = FreeImage_GetFileType(filename, 0);
////	//if still unknown, try to guess the file format from the file extension
////	if (fif == FIF_UNKNOWN)
////		fif = FreeImage_GetFIFFromFilename(filename);
////	//if still unkown, return failure
////	if (fif == FIF_UNKNOWN) {
////		TextureLoaded = false;
////		std::cout << "File Format Unknown" << std::endl;
////	}
////		
////
////	//check that the plugin has reading capabilities and load the file
////	if (FreeImage_FIFSupportsReading(fif))
////		dib = FreeImage_Load(fif, filename);
////	//if the image failed to load, return failure
////	if (!dib) {
////		TextureLoaded = false;
////		std::cout << "Image Failed to load" << std::endl;
////	}
////	//retrieve the image data
////	//tempTexture->data.insert(tempTexture->data.begin(),FreeImage_GetBits(dib));
////
////	//get the image width and height
////	tempTexture->width = FreeImage_GetWidth(dib);
////	
////	tempTexture->height = FreeImage_GetHeight(dib);
////
////	//FreeImage_Save(FIF_PNG, dib, "test.png", 0);
////	tempTexture->datatest = FreeImage_GetBits(dib);
////
////	FreeImage_GetDIBSize(dib);
////
////	tempTexture->data.insert(tempTexture->data.begin(), FreeImage_GetBits(dib), FreeImage_GetBits(dib) + FreeImage_GetDIBSize(dib));
////	
////	//IMAGE DATA TEST
////	
////	unsigned int pitch = FreeImage_GetPitch(dib);
////	unsigned int red = FreeImage_GetRedMask(dib);
////	unsigned int green = FreeImage_GetGreenMask(dib);
////	unsigned int blue = FreeImage_GetBlueMask(dib);
////	unsigned int bpp = FreeImage_GetBPP(dib);
////	std::cout << "DIB SIZE = " << FreeImage_GetDIBSize(dib) << std::endl;
////	dib = NULL;
////	dib = FreeImage_ConvertFromRawBits(&tempTexture->data[0], tempTexture->width, tempTexture->height,pitch, bpp, red, green, blue, 0);
////	FreeImage_Save(FIF_PNG, dib, "container.png", 0);
////
////
////
////	//if this somehow one of these failed (they shouldn't), return failure
////	//tempTexture->data.empty()
////	if ( tempTexture->data.empty()  || (tempTexture->width == 0) || (tempTexture->height == 0)) {
////		std::cout << "Everything Failed. Good Job!" << std::endl;
////		TextureLoaded = false;
////	}
////	std::cout << tempTexture->width << std::endl;
////	//if this texture ID is in use, unload the current texture
////	//glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
////	//	border, image_format, GL_UNSIGNED_BYTE, bits);
////
////	//Free FreeImage's copy of the data	
////	FreeImage_Unload(dib);
////
////	//return success
////	if (!TextureLoaded) {
////		std::cout << "Returning NULL..." << std::endl;
////		tempTexture = NULL;
////	}
////	return tempTexture;
////}
//
//
//
//void PMaterial::Serialize(Archive &Ar)
//{
//	std::cout << "mEmissive" << std::endl;
//
//	mEmissive->Serialize(Ar);
//	
//	std::cout << "mAmbient" << std::endl;
//	mAmbient->Serialize(Ar);
//	
//	std::cout << "mDiffuse" << std::endl;
//	mDiffuse->Serialize(Ar);
//	
//	std::cout << "mSpecular" << std::endl;
//	mSpecular->Serialize(Ar);
//
//	Ar.ArchiveWrite(&mShinness, sizeof(float));
//
//}
//
//void PMaterial::Deserialize(Archive & Ar)
//{
//	mEmissive->Deserialize(Ar);
//	mAmbient->Deserialize(Ar);
//	mDiffuse->Deserialize(Ar);
//	mSpecular->Deserialize(Ar);
//
//	Ar.ArchiveRead(&mShinness, sizeof(float));
//}
//
//
//void Channel::Serialize(Archive &Ar)
//{
//	Ar.ArchiveWrite(&ChName, sizeof(char*));
//	Ar.ArchiveWrite(&HasTexture, sizeof(bool));
//	if (HasTexture) {
//		ChannelTexture->Serialize(Ar);
//		//Ar.ArchiveWrite(&ChannelTexture,sizeof(RTexture));
//	}
//	Ar.ArchiveWrite(&Colour,sizeof(Vector4f));
//
//}
//
//void Channel::Deserialize(Archive & Ar)
//{
//	Ar.ArchiveRead(&ChName, sizeof(char*));
//	Ar.ArchiveRead(&HasTexture, sizeof(bool));
//	if (HasTexture) {
//		ChannelTexture->Deserialize(Ar);
//		//Ar.ArchiveRead(&ChannelTexture, sizeof(RTexture));
//	}
//	Ar.ArchiveRead(&Colour, sizeof(Vector4f));
//}
//
//
////Extract data
//void FbxLoader::Print() {
//
//	Archive *test = new Archive();
//	test->LoadToArchive("cube.asset");
//
//	std::vector<VertexFormat> ind;
//	size_t size = 0;
//	test->ArchiveRead(&size, sizeof(size_t));
//	ind = std::vector<VertexFormat>(size);
//	test->ArchiveRead(&ind[0], sizeof(VertexFormat)*size);
//
//	std::cout << ind[0].Position.X << std::endl;
//
//	delete test;
//}
//
//void RTexture::Serialize(Archive & Ar)
//{
//	//Ar.ArchiveWrite(&id, sizeof(unsigned int));
//	Ar.ArchiveWrite(&width, sizeof(unsigned int));
//	Ar.ArchiveWrite(&height, sizeof(unsigned int));
//
//	size_t dataSize = data.size();
//	Ar.ArchiveWrite(&dataSize,sizeof(size_t));
//	Ar.ArchiveWrite(&data[0], sizeof(byte)*dataSize);
//
//}
//
//void RTexture::Deserialize(Archive & Ar)
//{
//	// Ar.ArchiveRead(&id, sizeof(unsigned int));
//	Ar.ArchiveRead(&width, sizeof(unsigned int));
//	Ar.ArchiveRead(&height, sizeof(unsigned int));
//
//	size_t dataSize = 0;
//	Ar.ArchiveRead(&dataSize, sizeof(size_t));
//
//	data = std::vector<byte>(dataSize);
//	Ar.ArchiveRead(&data[0], sizeof(byte)*dataSize);
//
//}
//*/

ObjLoader::ObjLoader()
{}

ObjLoader::~ObjLoader()
{}


Asset * ObjLoader::Read(const char * Filepath)
{
	std::string filename, extension, directory;

	filename = FileUtilities::Normalise(Filepath);
	directory = FileUtilities::ParentPath(Filepath);
	filename = FileUtilities::FileStem(Filepath);
	extension = FileUtilities::FileExtension(Filepath);

	Vector3f temp = Vector3f();

	std::string line;

	//in.open(Filepath, std::ios::in);

	TArray<uint32> VertexIndices = TArray<uint32>();
	TArray<uint32> NormalIndices = TArray<uint32>();
	TArray<uint32> UVIndices = TArray<uint32>();

	TArray<uint32> Index;
	//Indices = IndexArray();

	uint32  Type = 0;


	///////////////////////////////////////////////////////////////////////
	// Parse the OBJ file using tinyobj
	///////////////////////////////////////////////////////////////////////
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	// Expect '.mtl' file in the same directory and triangulate meshes
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
		(directory + filename + extension).c_str(), directory.c_str(), true);
	if (!err.empty())
	{ // `err` may contain warning message.
		std::cerr << err << std::endl;
	}
	if (!ret)
	{
		//exit(1);
	}
	Asset* asset = new Asset();
	asset->name = Filepath;
	// model->m_filename = path;

	///////////////////////////////////////////////////////////////////////
	// Transform all materials into our datastructure
	///////////////////////////////////////////////////////////////////////
	for (const auto& m : materials)
	{
		PMaterial * material = new PMaterial();
		material->Name = m.name;

		//Diffuse
		material->Diffuse = PChannel();
		material->Diffuse.Colours = Vector4f(m.diffuse[0], m.diffuse[1], m.diffuse[2],1);
		if (m.diffuse_texname != "")
		{
			/*if (m.diffuse_texname.find("lion") != std::string::npos) {
				__debugbreak();
			}*/
			material->Diffuse.HasTexture = true;
			material->Diffuse.ChannelTexture = new RTexture();

			material->Diffuse.ChannelTexture->Generate(directory + m.diffuse_texname);
		}

		// Metalness
		material->Metalness = PChannel();
		material->Metalness.Colours = Vector4f(m.metallic, m.metallic, m.metallic, 1);
		if (m.metallic_texname != "")
		{
			material->Metalness.HasTexture = true;
			material->Metalness.ChannelTexture = new RTexture();

			material->Metalness.ChannelTexture->Generate(directory + m.metallic_texname);
		}

		//Fresnel
		material->Fresnel = PChannel();
		material->Fresnel.Colours = Vector4f(m.specular[0], m.specular[0], m.specular[0], 1);
		if (m.specular_texname != "")
		{
			material->Fresnel.HasTexture = true;
			material->Fresnel.ChannelTexture = new RTexture();

			material->Fresnel.ChannelTexture->Generate(directory + m.specular_texname);
		}

		//Roughness
		material->Roughness = PChannel();
		material->Roughness.Colours = Vector4f(m.roughness, m.roughness, m.roughness, 1);
		if (m.roughness_texname != "")
		{
			material->Roughness.HasTexture = true;
			material->Roughness.ChannelTexture = new RTexture();

			material->Roughness.ChannelTexture->Generate(directory + m.roughness_texname);
		}

		material->Emissive = PChannel();
		material->Emissive.Colours = Vector4f(m.emission[0], m.emission[1], m.emission[2], 1);
		if (m.emissive_texname != "")
		{
			material->Emissive.HasTexture = true;
			material->Emissive.ChannelTexture = new RTexture();

			material->Emissive.ChannelTexture->Generate(directory + m.emissive_texname);
		}

		material->Transparency = m.transmittance[0];
		material->IOR = m.ior;
		asset->Materials.PushBack(material);
	}

	///////////////////////////////////////////////////////////////////////
	// A vertex in the OBJ file may have different indices for position,
	// normal and texture coordinate. We will not even attempt to use
	// indexed lookups, but will store a simple vertex stream per mesh.
	///////////////////////////////////////////////////////////////////////
	uint64_t number_of_vertices = 0;
	for (const auto& shape : shapes)
	{
		number_of_vertices += shape.mesh.indices.size();
	}
	asset->Vertices = TArray<VertexFormat>(number_of_vertices, VertexFormat());


	///////////////////////////////////////////////////////////////////////
	// For each vertex _position_ auto generate a normal that will be used
	// if no normal is supplied.
	///////////////////////////////////////////////////////////////////////
	std::vector<glm::vec4> auto_normals(attrib.vertices.size() / 3);
	for (const auto& shape : shapes)
	{
		for (int face = 0; face < int(shape.mesh.indices.size()) / 3; face++)
		{
			glm::vec3 v0 = glm::vec3(attrib.vertices[shape.mesh.indices[face * 3 + 0].vertex_index * 3 + 0],
				attrib.vertices[shape.mesh.indices[face * 3 + 0].vertex_index * 3 + 1],
				attrib.vertices[shape.mesh.indices[face * 3 + 0].vertex_index * 3 + 2]);
			glm::vec3 v1 = glm::vec3(attrib.vertices[shape.mesh.indices[face * 3 + 1].vertex_index * 3 + 0],
				attrib.vertices[shape.mesh.indices[face * 3 + 1].vertex_index * 3 + 1],
				attrib.vertices[shape.mesh.indices[face * 3 + 1].vertex_index * 3 + 2]);
			glm::vec3 v2 = glm::vec3(attrib.vertices[shape.mesh.indices[face * 3 + 2].vertex_index * 3 + 0],
				attrib.vertices[shape.mesh.indices[face * 3 + 2].vertex_index * 3 + 1],
				attrib.vertices[shape.mesh.indices[face * 3 + 2].vertex_index * 3 + 2]);

			glm::vec3 e0 = glm::normalize(v1 - v0);
			glm::vec3 e1 = glm::normalize(v2 - v0);
			glm::vec3 face_normal = cross(e0, e1);

			auto_normals[shape.mesh.indices[face * 3 + 0].vertex_index] += glm::vec4(face_normal, 1.0f);
			auto_normals[shape.mesh.indices[face * 3 + 1].vertex_index] += glm::vec4(face_normal, 1.0f);
			auto_normals[shape.mesh.indices[face * 3 + 2].vertex_index] += glm::vec4(face_normal, 1.0f);
		}
	}
	for (auto& normal : auto_normals)
	{
		normal = (1.0f / normal.w) * normal;
	}

	///////////////////////////////////////////////////////////////////////
	// Now we will turn all shapes into Meshes. A shape that has several
	// materials will be split into several meshes with unique names
	///////////////////////////////////////////////////////////////////////
	int vertices_so_far = 0;
	for (int s = 0; s < shapes.size(); ++s)
	{
		const auto& shape = shapes[s];
		///////////////////////////////////////////////////////////////////
		// The shapes in an OBJ file may several different materials.
		// If so, we will split the shape into one Mesh per Material
		///////////////////////////////////////////////////////////////////
		int next_material_index = shape.mesh.material_ids[0];
		int next_material_starting_face = 0;
		std::vector<bool> finished_materials(materials.size(), false);
		int number_of_materials_in_shape = 0;
		while (next_material_index != -1)
		{
			int current_material_index = next_material_index;
			int current_material_starting_face = next_material_starting_face;
			next_material_index = -1;
			next_material_starting_face = -1;
			// Process a new Mesh with a unique material
			PStaticMesh * mesh = new PStaticMesh();
			mesh->Name = shape.name + "_" + materials[current_material_index].name;
			mesh->MaterialIndex = current_material_index;
			mesh->IndexStart = vertices_so_far;
			number_of_materials_in_shape += 1;

			uint64_t number_of_faces = shape.mesh.indices.size() / 3;
			Vector3f Min = Vector3f(std::numeric_limits<float>::infinity());
			Vector3f Max = Vector3f(-std::numeric_limits<float>::infinity());
			for (int i = current_material_starting_face; i < number_of_faces; i++)
			{
				if (shape.mesh.material_ids[i] != current_material_index)
				{
					if (next_material_index >= 0)
						continue;
					else if (finished_materials[shape.mesh.material_ids[i]])
						continue;
					else
					{ // Found a new material that we have not processed.
						next_material_index = shape.mesh.material_ids[i];
						next_material_starting_face = i;
					}
				}
				else
				{
					///////////////////////////////////////////////////////
					// Now we generate the vertices
					///////////////////////////////////////////////////////
					for (int j = 0; j < 3; j++)
					{
						int v = shape.mesh.indices[i * 3 + j].vertex_index;
						asset->Vertices[vertices_so_far + j].Position =
							Vector3f(attrib.vertices[shape.mesh.indices[i * 3 + j].vertex_index * 3 + 0],
								attrib.vertices[shape.mesh.indices[i * 3 + j].vertex_index * 3 + 1],
								attrib.vertices[shape.mesh.indices[i * 3 + j].vertex_index * 3 + 2]);

						Min.X = SMath::Min(Min.X, asset->Vertices[vertices_so_far + j].Position.X);
						Min.Y = SMath::Min(Min.Y, asset->Vertices[vertices_so_far + j].Position.Y);
						Min.Z = SMath::Min(Min.Z, asset->Vertices[vertices_so_far + j].Position.Z);

						Max.X = SMath::Max(Max.X, asset->Vertices[vertices_so_far + j].Position.X);
						Max.Y = SMath::Max(Max.Y, asset->Vertices[vertices_so_far + j].Position.Y);
						Max.Z = SMath::Max(Max.Z, asset->Vertices[vertices_so_far + j].Position.Z);
						
						if (shape.mesh.indices[i * 3 + j].normal_index == -1)
						{
							// No normal, use the autogenerated
							// asset->Vertices[vertices_so_far + j].Normal = Vector3f(
							//	auto_normals[shape.mesh.indices[i * 3 + j].vertex_index]);
						}
						else
						{
							asset->Vertices[vertices_so_far + j].Normal = Vector3f(attrib.normals[shape.mesh.indices[i * 3 + j].normal_index * 3 + 0],
									attrib.normals[shape.mesh.indices[i * 3 + j].normal_index * 3 + 1],
									attrib.normals[shape.mesh.indices[i * 3 + j].normal_index * 3 + 2]);
						}
						if (shape.mesh.indices[i * 3 + j].texcoord_index == -1)
						{
							// No UV coordinates. Use null.
							asset->Vertices[vertices_so_far + j].UVs = Vector2f(0.0f);
						}
						else
						{
							asset->Vertices[vertices_so_far + j].UVs = Vector2f(
								attrib.texcoords[shape.mesh.indices[i * 3 + j].texcoord_index * 2 + 0],
								attrib.texcoords[shape.mesh.indices[i * 3 + j].texcoord_index * 2 + 1]);
						}
					}
					vertices_so_far += 3;
				}
			}
			///////////////////////////////////////////////////////////////
			// Finalize and push this mesh to the list
			///////////////////////////////////////////////////////////////
			mesh->NumVertices = vertices_so_far - mesh->IndexStart;

			mesh->LocalBoundingBox->Min = Min;
			mesh->LocalBoundingBox->Max = Max;
			mesh->LocalBoundingBox->GetCornersFromMinMax();

			asset->Meshes.PushBack(mesh);
			finished_materials[current_material_index] = true;
		}
		if (number_of_materials_in_shape == 1)
		{
			// If there's only one material, we don't need the material name in the mesh name
			asset->Meshes.Back()->Name = shape.name;
		}
	}

	std::sort(asset->Meshes.Begin(), asset->Meshes.End(),
		[](const PStaticMesh *a, const PStaticMesh *b) { return a->Name < b->Name; });

	return asset;
}


const uint32 ObjLoader::Split(const std::string & in, const char * deli, TArray<uint32> & Index)
{
	std::string TempSubString, Token;

	uint32 VertexType = 1;
	//v		v/vt	v/vt/vn		v//vn
	for (int i = 0; i < in.length(); i++)
	{
		TempSubString = in.substr(i, 1);
		if (TempSubString.compare(deli) == 0)
		{
			if (!Token.empty())
			{
				Index.PushBack(std::stoi(Token) - 1);

				Token.clear();

			}
			else VertexType++;
			VertexType++;
		}
		else
		{
			Token += TempSubString;
		}
	}
	if (!Token.empty())
	{
		Index.PushBack(std::stoi(Token) - 1);
	}

	return VertexType;
}

Asset::Asset() : Vertices(), Indices()
{
}

Asset::~Asset()
{
	
	Vertices.~TArray<VertexFormat>();
	Indices.~TArray<uint32>();
	Materials.~TArray<PMaterial *>();
	Meshes.~TArray<PStaticMesh *>();
}

bool Asset::isEmpty()
{
	uint8 Result = false;
	if (Vertices.IsEmpty() || Indices.IsEmpty())
	{
		Result = true;
	}

	return Result;
}


//
//static bool parseGeometryMaterials(GeometryDataImpl& geom, const Element& element, std::vector<ParseDataJob>& jobs)
//}