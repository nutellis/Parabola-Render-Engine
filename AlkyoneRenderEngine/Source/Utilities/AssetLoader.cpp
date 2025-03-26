#include <Utilities/AssetLoader.h>

#include <Managers/LogManager.h>

#include <Components/Material.h>





template<> GAssetLoader* SingletonBase<GAssetLoader>::instance = 0;
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

const char* GAssetLoader::SplitPath(const char * filepath)
{
	std::string temp;
	char* tempchar;

	int i = 0, pos = 0, j = 0, count = 0, size = 0;;
	while (filepath[i] != '\0' && filepath[i] != '.')
	{
		if (filepath[i] == '/')
		{
			pos = i;
			count = 0;
		}
		if (filepath[i + 1] == '.')
		{
			size = count;
		}
		count++;
		i++;
	}
	temp.resize(size);

	i = pos + 1;
	while (filepath[i] != '.')
	{
		temp[j] = filepath[i];
		j++;
		i++;
	}
	temp += ".asset";
	tempchar = new char[temp.size()];
	strcpy(tempchar, temp.c_str());
	return (const char*)tempchar;
}



Asset * GAssetLoader::LoadAsset(const char * Filepath)
{
	Asset * Result = new Asset();

	std::string Extension, TempStr = Filepath;
	size_t pos = TempStr.find_first_of(".");
	Extension = TempStr.substr(pos, std::string::npos);
	LOG(DEBUG, "IMPORTING MESH...");
	if (Extension.compare(".obj") == 0)
	{
		ObjLoader Loader;// = ObjLoader();

		Result = Loader.Read(Filepath);
	}
	else if (Extension.compare(".fbx") == 0)
	{
		FbxLoader Loader;// = FbxLoader();

		Result = Loader.Read(Filepath);

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
			format->Materials = FetchMaterial(node);

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

			FbxLayerElementNormal * LayerElementNormal = BaseLayer->GetNormals();

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

			Vector3f VertexPosition(0.0f);

			for (int32 VertexIndex = 0; VertexIndex <= VertexCount; ++VertexIndex)
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

			//number of polugons(triangles for us) For each polygon
			for (int32 PolygonIndex = 0; PolygonIndex < PolygonCount; PolygonIndex++)
			{

				int32 PolygonVertexCount = Mesh->GetPolygonSize(PolygonIndex);

				Vector3f position(0.0f);
				Vector3f normal(0.0f);
				Vector2f uv(0.0f);

				MaterialPerPolygon.PushBack(LayerElementMaterial->GetIndexArray()[PolygonIndex]);
				// MaterialIndexMapping[LayerElementMaterial->GetIndexArray()[PolygonIndex]] = TArray<uint32>();

				//Vector3f binormal(0.0f);
				//Vector3f tangent(0.0f);
				//indexes(corners) per polygon (triangles for us, actually vertices). For each vertex per polygon
				for (int32 CornerIndex = 0; CornerIndex < PolygonVertexCount; CornerIndex++, RealFbxVertexIndex++)
				{
					ControlPointIndex = Mesh->GetPolygonVertex(PolygonIndex, CornerIndex);

					Positions.PushBack(RawPositions[ControlPointIndex]);

					if (LayerElementMaterial) {
						switch (MaterialMappingMode) {
						case FbxLayerElement::eAllSame: {
							MaterialIndexMapping[LayerElementMaterial->GetIndexArray().GetAt(0)].PushBack(ControlPointIndex);
						}
						break;
						case FbxLayerElement::eByPolygon: {
							MaterialIndexMapping[LayerElementMaterial->GetIndexArray().GetAt(PolygonIndex)].PushBack(ControlPointIndex);
						}
						break;
						}
					}
					

					//Fetching Normals
					if (LayerElementNormal)
					{

						int32 NormalMapIndex, NormalValueIndex;

						if (NormalMappingMode == FbxLayerElement::eByControlPoint)
						{
							NormalMapIndex = ControlPointIndex;
						}
						else
						{
							NormalMapIndex = RealFbxVertexIndex;
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
  			InsertVertex(hasUVs, hasNormals, false);
			//actually use this data


			format->Vertex = Vertices;
			format->indices = Indices;
			format->MaterialIndexMapping = MaterialIndexMapping;
			//format->BoundingBox.SetExtents(Min, Max);

			//format.material = mat;

			std::cout << MaterialIndexMapping[1].Size() << "\n";
			for (auto i = 0; i < MaterialIndexMapping[1].Size(); i++) {
				std::cout << MaterialIndexMapping[1][i] << "\t";
			}

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

		NewMaterial.Ambient = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);

		NewMaterial.Diffuse = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);

		NewMaterial.Specular = *GetMaterialProperty(FbxMaterial,
			FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);

		FbxProperty lShininessProperty = FbxMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
		if (lShininessProperty.IsValid())
		{
			double lShininess = lShininessProperty.Get<FbxDouble>();
			NewMaterial.Shinness = static_cast<float>(lShininess);
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
		lResult->Colour = Vector4f((float)lColour[0], (float)lColour[1], (float)lColour[2], 1.0f);
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

//void BaseLoader::InsertVertex(const Vector3f & position, const Vector3f & normal, const Vector2f & uv)
//{
//	VertexFormat Vertex = {position, normal, uv};
//	auto lookup = s_IndexMapping.find(Vertex);
//	if (lookup != s_IndexMapping.end())
//	{
//		//std::cout << "OLD VERTEX\n";// << Vertex.Normal.X << "\t" << Vertex.Normal.Y << "\t" << Vertex.Normal.Z << "\n";
//		
//		s_Indices.PushBack(lookup->second);
//	}
//	else
//	{
//		//std::cout << "NEW VERTEX\n";// << Vertex.Normal.X << "\t" << Vertex.Normal.Y << "\t" << Vertex.Normal.Z << "\n";
//		
//		uint32 index = static_cast<uint32>(s_Vertices.Size());
//		s_IndexMapping[Vertex] = index;
//		s_Indices.PushBack(index);
//		s_Vertices.PushBack(Vertex);
//	}
//}

void BaseLoader::InsertVertex(bool hasUVs, bool hasNormals, bool doIndices)
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

		if (doIndices) {
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
		}

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
////Texture *PMaterial::LoadTexture(const char * filename) //, GLenum image_format, int internal_format, int level, int border
////{
////	Texture *tempTexture = new Texture();
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
//		//Ar.ArchiveWrite(&ChannelTexture,sizeof(Texture));
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
//		//Ar.ArchiveRead(&ChannelTexture, sizeof(Texture));
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
//void Texture::Serialize(Archive & Ar)
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
//void Texture::Deserialize(Archive & Ar)
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
	std::ifstream in;

	Vector3f temp = Vector3f();

	Asset * format = new Asset();

	std::string line;

	in.open(Filepath, std::ios::in);

	TArray<uint32> VertexIndices = TArray<uint32>();
	TArray<uint32> NormalIndices = TArray<uint32>();
	TArray<uint32> UVIndices = TArray<uint32>();

	TArray<uint32> Index;
	//Indices = IndexArray();

	uint32  Type = 0;

	while (in)
	{
		//std::getline(in, line);

		in >> line;

		if (line.compare("v") == 0)
		{
			in >> temp.X >> temp.Y >> temp.Z;

			RawPositions.PushBack(temp);
		}
		else if (line.compare("vt") == 0)
		{
			in >> temp.X >> temp.Y;

			RawUVs.PushBack(temp);
		}
		else if (line.compare("vn") == 0)
		{
			in >> temp.X >> temp.Y >> temp.Z;

			RawNormals.PushBack(temp);
		}
		else if (line.compare("f") == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				in >> line;
				

				Type = Split(line, "/", Index);

				VertexIndices.PushBack(Index[0]);

				// v	v/vt	v//vn	v/vt/vn
				if (Type == 1)
				{
					hasNormals = false;
					hasUVs = false;
				}
				else if (Type == 2)
				{

					UVIndices.PushBack(Index[1]);

					hasNormals = false;
					hasUVs = true;
				}
				else if (Type == 3)
				{
					UVIndices.PushBack(Index[1]);
					NormalIndices.PushBack(Index[2]);

					hasNormals = true;
					hasUVs = true;
				}
				else if (Type == 4)
				{
					NormalIndices.PushBack(Index[1]);

					hasNormals = true;
					hasUVs = false;
				}
				else
				{
					//error, not good
				}

				Index.Clear();
			}
		}
	}

	for (int i = 0; i < VertexIndices.Size(); i++)
	{
		Positions.PushBack(RawPositions[VertexIndices[i]]);


		if (hasUVs) {
			UVs.PushBack(RawUVs[UVIndices[i]]);
		}
		if (hasNormals)
		{
			Normals.PushBack(RawNormals[NormalIndices[i]]);
		}
	}


	InsertVertex(hasUVs, hasNormals, true);


	format->Vertex = Vertices;
	format->indices = Indices;



	return format;
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

Asset::Asset() : Vertex(), indices()
{
}

Asset::~Asset()
{
	Vertex.~TArray<VertexFormat>();
	indices.~TArray<uint32>();
	Materials.~TArray<PMaterial>();
}

bool Asset::isEmpty()
{
	uint8 Result = false;
	if (Vertex.IsEmpty() || indices.IsEmpty())
	{
		Result = true;
	}

	return Result;
}


//
//static bool parseGeometryMaterials(GeometryDataImpl& geom, const Element& element, std::vector<ParseDataJob>& jobs)
//}