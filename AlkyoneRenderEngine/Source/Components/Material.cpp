#include <Components/Material.h>
#include <Components/Texture.h>


RChannel::RChannel() :
	HasTexture(false),
	ChannelTexture(nullptr), 
	Colours(Vector4f())
{}

// Copy constructor
RChannel::RChannel(const RChannel& Other)
	: Name(Other.Name),
	Colours(Other.Colours),
	HasTexture(Other.HasTexture),
	ChannelTexture(Other.ChannelTexture ? new RTexture(*Other.ChannelTexture) : nullptr)
{
}

// Move constructor
RChannel::RChannel(RChannel&& Other) noexcept
	: Name(Utilities::Move(Other.Name)),
	Colours(Other.Colours),
	HasTexture(Other.HasTexture),
	ChannelTexture(Other.ChannelTexture)
{
	Other.ChannelTexture = nullptr;
}

RChannel::~RChannel()
{
}

// Copy assignment
RChannel& RChannel::operator=(const RChannel& Other)
{
	if (this == &Other) return *this;
	Name = Other.Name;
	Colours = Other.Colours;
	HasTexture = Other.HasTexture;
	if (ChannelTexture) delete ChannelTexture;
	ChannelTexture = Other.ChannelTexture ? new RTexture(*Other.ChannelTexture) : nullptr;
	return *this;
}

// Move assignment
RChannel& RChannel::operator=(RChannel&& Other) noexcept
{
	if (this == &Other) return *this;
	Name = Other.Name;
	Colours = Other.Colours;
	HasTexture = Other.HasTexture;

	if (ChannelTexture) delete ChannelTexture;

	ChannelTexture = Other.ChannelTexture;
	Other.ChannelTexture = nullptr;
	return *this;
}


RMaterial::RMaterial() : ShaderName("BRDF")
{	//TextureSlot = 0;
	//isActive = false;
}

//Copy Constructor
RMaterial::RMaterial(const RMaterial& Other)
	: Name(Other.Name),
	MaterialID(Other.MaterialID),
	ShaderName(Other.ShaderName),
	Diffuse(Other.Diffuse),
	Metalness(Other.Metalness),
	Roughness(Other.Roughness),
	Fresnel(Other.Fresnel),
	Emissive(Other.Emissive),
	Specular(Other.Specular),
	Normal(Other.Normal),
	Transparency(Other.Transparency),
	IOR(Other.IOR),
	HasTransparency(Other.HasTransparency)
{
}


// Move constructor
RMaterial::RMaterial(RMaterial&& Other) noexcept
	: Name(Utilities::Move(Other.Name)),
	MaterialID(Other.MaterialID),
	ShaderName(Utilities::Move(Other.ShaderName)),
	Diffuse(Utilities::Move(Other.Diffuse)),
	Metalness(Utilities::Move(Other.Metalness)),
	Roughness(Utilities::Move(Other.Roughness)),
	Fresnel(Utilities::Move(Other.Fresnel)),
	Emissive(Utilities::Move(Other.Emissive)),
	Specular(Utilities::Move(Other.Specular)),
	Normal(Utilities::Move(Other.Normal)),
	Transparency(Other.Transparency),
	IOR(Other.IOR)
{
}

//RMaterial::RMaterial(UArchive &Ar) : mShinness(0)
//{
//	mEmissive = new Channel();
//	mAmbient = new Channel();
//	mDiffuse = new Channel();
//	mSpecular = new Channel();
//
//	TextureSlot = 0;
//	isActive = false;
//
//	Deserialize(Ar);
//}


RMaterial::~RMaterial()
{
	//delete Emissive;
	//delete Ambient;
	//delete Diffuse;
	//delete Specular;
}


// Copy assignment
RMaterial& RMaterial::operator=(const RMaterial& Other)
{
	if (this == &Other) return *this;

	Name = Other.Name;
	MaterialID = Other.MaterialID;
	ShaderName = Other.ShaderName;
	Diffuse = Other.Diffuse;
	Metalness = Other.Metalness;
	Roughness = Other.Roughness;
	Fresnel = Other.Fresnel;
	Emissive = Other.Emissive;
	Specular = Other.Specular;
	Normal = Other.Normal;
	Transparency = Other.Transparency;
	IOR = Other.IOR;

	return *this;
}


// Move assignment
RMaterial& RMaterial::operator=(RMaterial&& Other) noexcept
{
	if (this == &Other) return *this;

	Name = Utilities::Move(Other.Name);
	MaterialID = Other.MaterialID;
	ShaderName = Utilities::Move(Other.ShaderName);
	Diffuse = Utilities::Move(Other.Diffuse);
	Metalness = Utilities::Move(Other.Metalness);
	Roughness = Utilities::Move(Other.Roughness);
	Fresnel = Utilities::Move(Other.Fresnel);
	Emissive = Utilities::Move(Other.Emissive);
	Specular = Utilities::Move(Other.Specular);
	Normal = Utilities::Move(Other.Normal);
	Transparency = Other.Transparency;
	IOR = Other.IOR;

	return *this;
}


/*
void RMaterial::SetupMaterial() 
{
	mEmissive->SetupChannel();
	mAmbient->SetupChannel();
	mDiffuse->SetupChannel();
	mSpecular->SetupChannel();
}

void RMaterial::SetActiveMaterial(Shader s) 
{
	mEmissive->SetShader(s,TextureSlot);
	mAmbient->SetShader(s, TextureSlot);
	mDiffuse->SetShader(s, TextureSlot);
	mSpecular->SetShader(s, TextureSlot);

	s.setFloat("Shinness", mShinness);

	TextureSlot = 0;
	isActive = true;
}

void RMaterial::Serialize(UArchive &Ar)
{
	mEmissive->Serialize(Ar);
	mAmbient->Serialize(Ar);
	mDiffuse->Serialize(Ar);
	mSpecular->Serialize(Ar);

	Ar.ArchiveWrite(&mShinness, sizeof(float));

}

void RMaterial::Deserialize(UArchive & Ar)
{
	std::cout << "Emmisive" << std::endl;
	mEmissive->Deserialize(Ar);
	std::cout << "Ambient" << std::endl;
	mAmbient->Deserialize(Ar);
	std::cout << "Diffuse" << std::endl;
	mDiffuse->Deserialize(Ar);
	std::cout << "Specular" << std::endl;
	mSpecular->Deserialize(Ar);

	Ar.ArchiveRead(&mShinness, sizeof(float));
}

void RTexture::SetupTextures()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, &data[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Channel::SetupChannel()
{
	if (HasTexture) {
		ChannelTexture->SetupTextures();
	}
}

void Channel::SetShader(Shader s,uint32 &TextureSlot)
{
	if (HasTexture) {
		glActiveTexture(GL_TEXTURE0+TextureSlot);
		glBindTexture(GL_TEXTURE_2D, ChannelTexture->textureID);
		TextureSlot++;
	}
	//s.setVec4(ChName, Colour);
}

void Channel::Serialize(UArchive &Ar)
{
	Ar.ArchiveWrite(&ChName, sizeof(char*));
	Ar.ArchiveWrite(&HasTexture, sizeof(bool));
	if (HasTexture) {
		//ChannelTexture->Serialize(Ar)
		//ChannelTexture = new RTexture();
		//Ar.ArchiveWrite(&ChannelTexture, sizeof(RTexture));
	}
	Ar.ArchiveWrite(&Colour, sizeof(glm::vec4));

}


void Channel::Deserialize(UArchive & Ar)
{
	Ar.ArchiveRead(&ChName, sizeof(char*));
	Ar.ArchiveRead(&HasTexture, sizeof(bool));
	if (HasTexture) {
		ChannelTexture = new RTexture();
		ChannelTexture->Deserialize(Ar);
		//Ar.ArchiveRead(&ChannelTexture, sizeof(RTexture));
	}
	Ar.ArchiveRead(&Colour, sizeof(glm::vec4));
}


void RTexture::Deserialize(UArchive & Ar)
{
	//Ar.ArchiveRead(&textureID, sizeof(uint32));
	Ar.ArchiveRead(&width, sizeof(uint32));
	Ar.ArchiveRead(&height, sizeof(uint32));

	size_t dataSize = 0;
	Ar.ArchiveRead(&dataSize, sizeof(size_t));

	std::cout << dataSize << std::endl;

	data = std::vector<byte>(dataSize);
	Ar.ArchiveRead(&data[0], sizeof(byte)*dataSize);

}*/



