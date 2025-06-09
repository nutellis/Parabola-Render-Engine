#pragma once

#include <Core/RenderCore.h>
#include <Utilities/Containers/Array.h>

class Image;

struct RTextureOptions {
	uint32 Target;				// what type of texture we're dealing with
	uint32 InternalFormat;      // number of color components
	uint32 Format;				// the format each texel is stored in
	uint32 Type;
	uint32 FilterMin;			// what filter method to use during minification
	uint32 FilterMag;           // what filter method to use during magnification
	uint32 WrapS;				// wrapping method of the S coordinate
	uint32 WrapT;;				// wrapping method of the T coordinate
	uint32 WrapR;				// wrapping method of the R coordinate

	uint32 CompareMode;
	uint32 CompareFunc;

	bool EnableMipMapping;

	//RTextureOptions();
	RTextureOptions(
		uint32 InTarget = GL_TEXTURE_2D,
		uint32 InInternalFormat = GL_RGB8,
		uint32 InFormat = GL_RGB,
		uint32 InType = GL_UNSIGNED_BYTE,
		uint32 InFilterMin = GL_LINEAR_MIPMAP_LINEAR,
		uint32 InFilterMag = GL_LINEAR,
		uint32 InWrapS = GL_REPEAT,
		uint32 InWrapT = GL_REPEAT,
		uint32 InWrapR = GL_REPEAT,
		uint32 InCompareMode = GL_NONE,
		uint32 InCompareFunc = GL_LESS,
		bool bEnableMipMapping = GL_FALSE
	);

	static RTextureOptions InitForShadows();
	static RTextureOptions InitDefault();
	static RTextureOptions InitDefaultDepth();
};

class RTexture {
public:
	RTexture();
	RTexture(uint32 Width, uint32 Height, RTextureOptions Options);

	RTexture(const RTexture& other);
	RTexture(RTexture&& other) noexcept;

	~RTexture();

	RTexture& operator=(const RTexture& Other);

	RTexture& operator=(RTexture&& Other) noexcept;

	void Generate(std::string filename, bool IsHDRImage = false);

	void GenerateTextureLayers(TArray<std::string> filenames, bool IsHDRImage = false);

	void Generate();

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
	RTextureOptions TextureOptions;
	Image * TextureImage;
	
	void SetupTextures();


};

