#pragma once

#include <Core/UDTypes.h>

#include <ParabolaMath.h>

class PChannel
{
public:
	const char* Name;

	Vector4f Colour;

	bool HasTexture;
	
	Texture *ChannelTexture;

public:
	PChannel();
	~PChannel();

	//void Serialize(Archive &Ar);
	//void Deserialize(Archive &Ar);
};

class PMaterial
{
public:
	PMaterial();
	~PMaterial();
	//PMaterial *Initialize(const FbxSurfaceMaterial * pMaterial);

	// Set material colors and binding diffuse texture if exists.
	//void SetCurrentMaterial() const;
	//Channel *GetMaterialProperty(const FbxSurfaceMaterial * pMaterial,
	//	const char * pPropertyName,
		//const char * pFactorPropertyName);

	//Texture *LoadTexture(const char* filename);
	//void Serialize(Archive &Ar);
	//void Deserialize(Archive &Ar);



//private:

	//TODO: Add roughness and more later
	PChannel Emissive;
	PChannel Ambient;
	PChannel Diffuse;
	PChannel Specular;
	PChannel Normal;
	float Shinness;

	uint32 ShaderID;
};