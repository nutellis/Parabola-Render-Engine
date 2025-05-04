#pragma once

#include <Core/RenderCore.h>
#include <Utilities/Containers/Array.h>

class Image;

class Texture {
public:
	Texture();
	Texture(uint32 Width, uint32 Height);

	~Texture();

	void Generate(std::string filename, bool IsHDRImage = false);

	void GenerateTextureLayers(TArray<std::string> filenames, bool IsHDRImage = false);

	void GenerateDepthTexture();

	void GenerateColourTexture();

	// 2D texture generation
	void Generate(uint32 iNwidth, uint32 iNheight, uint32 inInternalFormat, uint32 InFormat, uint32 InType, void* InData);

	void Bind(int32 Unit = -1);
	void Unbind();

	// update relevant texture state
	void SetWrapModeS(uint32 WrapMode, uint8 Bind = false);
	void SetWrapModeT(uint32 WrapMode, uint8 Bind = false);
	void SetFilterMin(uint32 Filter, uint8 Bind = false);
	void SetFilterMag(uint32 Filter, uint8 Bind = false);

public:

	uint64 TextureHandle;
	uint32 TextureID;
	uint32 Width;
	uint32 Height;

	Image * TextureImage;
	
	void SetupTextures();

private:

	uint32 Target;				// what type of texture we're dealing with
	uint32 InternalFormat;      // number of color components
	uint32 Format;				// the format each texel is stored in
	uint32 Type;
	uint32 FilterMin;			// what filter method to use during minification
	uint32 FilterMag;           // what filter method to use during magnification
	uint32 WrapS;				// wrapping method of the S coordinate
	uint32 WrapT;;				// wrapping method of the T coordinate
	uint32 WrapR;				// wrapping method of the R coordinate
	uint8 Mipmapping : 1;




};

