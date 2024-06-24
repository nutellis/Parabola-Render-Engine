#include <Components/Material.h>



PChannel::PChannel() :
	HasTexture(false),
	ChannelTexture(nullptr), 
	Colour(Vector4f())
{}

PChannel::~PChannel()
{
}




PMaterial::PMaterial() :
	Emissive(),
	Ambient(),
	Diffuse(),
	Specular(),
	Normal(),
	Shinness(0)
{	//TextureSlot = 0;
	//isActive = false;
}

//PMaterial::PMaterial(UArchive &Ar) : mShinness(0)
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


PMaterial::~PMaterial()
{
	/*delete Emissive;
	delete Ambient;
	delete Diffuse;
	delete Specular;*/
}
/*
void PMaterial::SetupMaterial() 
{
	mEmissive->SetupChannel();
	mAmbient->SetupChannel();
	mDiffuse->SetupChannel();
	mSpecular->SetupChannel();
}

void PMaterial::SetActiveMaterial(Shader s) 
{
	mEmissive->SetShader(s,TextureSlot);
	mAmbient->SetShader(s, TextureSlot);
	mDiffuse->SetShader(s, TextureSlot);
	mSpecular->SetShader(s, TextureSlot);

	s.setFloat("Shinness", mShinness);

	TextureSlot = 0;
	isActive = true;
}

void PMaterial::Serialize(UArchive &Ar)
{
	mEmissive->Serialize(Ar);
	mAmbient->Serialize(Ar);
	mDiffuse->Serialize(Ar);
	mSpecular->Serialize(Ar);

	Ar.ArchiveWrite(&mShinness, sizeof(float));

}

void PMaterial::Deserialize(UArchive & Ar)
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

void Texture::SetupTextures()
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
		//ChannelTexture = new Texture();
		//Ar.ArchiveWrite(&ChannelTexture, sizeof(Texture));
	}
	Ar.ArchiveWrite(&Colour, sizeof(glm::vec4));

}


void Channel::Deserialize(UArchive & Ar)
{
	Ar.ArchiveRead(&ChName, sizeof(char*));
	Ar.ArchiveRead(&HasTexture, sizeof(bool));
	if (HasTexture) {
		ChannelTexture = new Texture();
		ChannelTexture->Deserialize(Ar);
		//Ar.ArchiveRead(&ChannelTexture, sizeof(Texture));
	}
	Ar.ArchiveRead(&Colour, sizeof(glm::vec4));
}


void Texture::Deserialize(UArchive & Ar)
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

