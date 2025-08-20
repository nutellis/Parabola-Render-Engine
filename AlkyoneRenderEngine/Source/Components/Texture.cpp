#include <Components\Texture.h>
#include <Components/Image.h>

RTexture::RTexture() : TextureID(0), Width(0), Height(0), TextureOptions()
{}

RTexture::RTexture(uint32 InWidth, uint32 InHeight, RTextureOptions TextureOptions): TextureID(0), Width(InWidth), Height(InHeight), TextureOptions(TextureOptions), TextureImage()
{}

// Copy constructor
RTexture::RTexture(const RTexture& Other)
    : TextureHandle(Other.TextureHandle),
    TextureID(Other.TextureID),
    Width(Other.Width),
    Height(Other.Height),
    TextureImage(Other.TextureImage ? new Image(*Other.TextureImage) : nullptr),
    TextureOptions(Other.TextureOptions)
{
}

// Move constructor
RTexture::RTexture(RTexture&& Other) noexcept
    : TextureHandle(Other.TextureHandle),
    TextureID(Other.TextureID),
    Width(Other.Width),
    Height(Other.Height),
    TextureImage(Other.TextureImage),
    TextureOptions(Other.TextureOptions)
{
    Other.TextureImage = nullptr;
}

RTexture::~RTexture()
{
    glDeleteTextures(1, &TextureID);
}

// Copy assignment
RTexture& RTexture::operator=(const RTexture& Other)
{
    if (this == &Other) return *this;

    TextureHandle = Other.TextureHandle;
    TextureID = Other.TextureID;
    Width = Other.Width;
    Height = Other.Height;
    
    if (TextureImage) delete TextureImage;
    TextureImage = Other.TextureImage ? new Image(*Other.TextureImage) : nullptr;
    
    TextureOptions = Other.TextureOptions;

    return *this;
}

// Move assignment
RTexture& RTexture::operator=(RTexture&& Other) noexcept
{
    if (this == &Other) return *this;

    TextureHandle = Other.TextureHandle;
    TextureID = Other.TextureID;
    Width = Other.Width;
    Height = Other.Height;
   
    if (TextureImage) delete TextureImage;
    TextureImage = Other.TextureImage;
    Other.TextureImage = nullptr;
   
    TextureOptions = Other.TextureOptions;
    return *this;
}

void RTexture::Generate(std::string filename, bool IsHDRImage)
{
    glCreateTextures(TextureOptions.Target, 1, &TextureID);

    glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, TextureOptions.FilterMin);
    glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, TextureOptions.FilterMag);

    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, TextureOptions.WrapS);
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, TextureOptions.WrapT);

    TextureImage = new Image(filename.c_str(), true, IsHDRImage);

    Width = TextureImage->Width;
    Height = TextureImage->Height;

    if (IsHDRImage) {
        glTextureStorage2D(TextureID, 1, GL_RGB32F, Width, Height);
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureStorage2D(TextureID, 1, GL_RGBA8, Width, Height);
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());
    }
    if (TextureOptions.EnableMipMapping == GL_TRUE)
    {
        glGenerateTextureMipmap(TextureID);
    }
}

void RTexture::GenerateTextureLayers(TArray<std::string> filenames, bool IsHDRImage)
{
    glCreateTextures(TextureOptions.Target, 1, &TextureID);

    glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, TextureOptions.FilterMin);
    glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, TextureOptions.FilterMag);

    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, TextureOptions.WrapS);
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, TextureOptions.WrapT);

    TextureImage = new Image(filenames[0].c_str(), true, IsHDRImage);

    Width = TextureImage->Width;
    Height = TextureImage->Height;

    if (IsHDRImage) {
        glTextureStorage2D(TextureID, filenames.Size(), GL_RGB32F, Width, Height);
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureStorage2D(TextureID, filenames.Size(), GL_RGB8, Width, Height);
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());
    }
    glGenerateTextureMipmap(TextureID);

    if (IsHDRImage) {
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());

    }

    for (int i = 1; i < filenames.Size(); i++)
    {
        Image image(filenames[i].c_str(), true, IsHDRImage);
        
        if (IsHDRImage) {
            glTextureSubImage2D(TextureID, i, 0, 0, image.Width, image.Height, GL_RGB, GL_FLOAT, image.GetDataFloat());
        }
        else {
            glTextureSubImage2D(TextureID, i, 0, 0, image.Width, image.Height, GL_RGB, GL_UNSIGNED_BYTE, image.GetDataUint8());
        }

    }
}

void RTexture::Generate() {
    glCreateTextures(TextureOptions.Target, 1, &TextureID);
    CHECK_GL_ERROR();
    glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, TextureOptions.FilterMin);
    glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, TextureOptions.FilterMag);
    CHECK_GL_ERROR();
    glTextureParameteri(TextureID, GL_TEXTURE_COMPARE_MODE, TextureOptions.CompareMode);
    glTextureParameteri(TextureID, GL_TEXTURE_COMPARE_FUNC, TextureOptions.CompareFunc);
    CHECK_GL_ERROR();
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, TextureOptions.WrapS);
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, TextureOptions.WrapT);
    CHECK_GL_ERROR();
    glTextureStorage2D(TextureID, 1, TextureOptions.InternalFormat, Width, Height);

    //TextureHandle = glGetTextureHandleARB(TextureID);
	CHECK_GL_ERROR();

}

void RTexture::Bind(int32 Unit)
{
    if (Unit >= 0)
    {
        glActiveTexture(GL_TEXTURE0 + Unit);
    }
    glBindTexture(TextureOptions.Target, TextureID);
}

void RTexture::Unbind()
{
    glBindTexture(TextureOptions.Target, 0);
}

void RTexture::SetWrapModeS(uint32 WrapMode, uint8 ShouldBind)
{
    TextureOptions.WrapS = WrapMode;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(TextureOptions.Target, GL_TEXTURE_WRAP_S, TextureOptions.WrapS);
        Unbind();
    }
}

void RTexture::SetWrapModeT(uint32 WrapMode, uint8 ShouldBind)
{
    TextureOptions.WrapT = WrapMode;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(TextureOptions.Target, GL_TEXTURE_WRAP_T, TextureOptions.WrapT);
        Unbind();
    }
}

void RTexture::SetFilterMin(uint32 Filter, uint8 ShouldBind)
{
    TextureOptions.FilterMin = Filter;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(TextureOptions.Target, GL_TEXTURE_MIN_FILTER, TextureOptions.FilterMin);
        Unbind();
    }
}

void RTexture::SetFilterMag(uint32 Filter, uint8 ShouldBind)
{
    TextureOptions.FilterMag = Filter;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(TextureOptions.Target, GL_TEXTURE_MAG_FILTER, TextureOptions.FilterMag);
        Unbind();
    }
}

//RTextureOptions::RTextureOptions()
//{
//    Target = GL_TEXTURE_2D;
//    InternalFormat = GL_RGB8;
//    Format = GL_RGB;
//    Type = GL_UNSIGNED_BYTE;
//    FilterMin = GL_LINEAR_MIPMAP_LINEAR;
//    FilterMag = GL_LINEAR;
//    WrapS = GL_REPEAT;
//    WrapT = GL_REPEAT;
//    WrapR = GL_REPEAT;
//
//	CompareMode = GL_NONE;
//	CompareFunc = GL_LESS;
//
//    EnableMipMapping = GL_FALSE;
//}

RTextureOptions::RTextureOptions(uint32 InTarget, uint32 InInternalFormat, uint32 InFormat, uint32 InType,
	uint32 InFilterMin, uint32 InFilterMag, uint32 InWrapS, uint32 InWrapT, uint32 InWrapR,
	uint32 InCompareMode, uint32 InCompareFunc, bool bEnableMipMapping)
	: Target(InTarget), InternalFormat(InInternalFormat), Format(InFormat), Type(InType),
	FilterMin(InFilterMin), FilterMag(InFilterMag), WrapS(InWrapS), WrapT(InWrapT), WrapR(InWrapR),
	CompareMode(InCompareMode), CompareFunc(InCompareFunc), EnableMipMapping(bEnableMipMapping)
{
}

RTextureOptions RTextureOptions::InitForShadows()
{
	RTextureOptions ShadowOptions = RTextureOptions();

    ShadowOptions.InternalFormat = GL_DEPTH_COMPONENT32F;
    ShadowOptions.FilterMin = GL_LINEAR;
    ShadowOptions.FilterMag = GL_LINEAR;
    ShadowOptions.CompareMode = GL_NONE;
    ShadowOptions.CompareFunc = GL_LESS;
    ShadowOptions.WrapS = GL_CLAMP_TO_BORDER;
    ShadowOptions.WrapT = GL_CLAMP_TO_BORDER;

    return ShadowOptions;
}

RTextureOptions RTextureOptions::InitDefault()
{
    RTextureOptions DefaultOptions = RTextureOptions();

    DefaultOptions.InternalFormat = GL_RGB8;
    DefaultOptions.FilterMin = GL_LINEAR;
    DefaultOptions.FilterMag = GL_LINEAR;
    DefaultOptions.WrapS = GL_CLAMP_TO_EDGE;
    DefaultOptions.WrapT = GL_CLAMP_TO_EDGE;
    DefaultOptions.WrapR = GL_REPEAT;

    return DefaultOptions;
}

RTextureOptions RTextureOptions::InitDefaultDepth()
{
    RTextureOptions DefaultDepthOptions = RTextureOptions();

    DefaultDepthOptions.InternalFormat = GL_DEPTH_COMPONENT32F;
    DefaultDepthOptions.CompareMode = GL_NONE;
    DefaultDepthOptions.CompareFunc = GL_LESS;
    DefaultDepthOptions.WrapS = GL_CLAMP_TO_EDGE;
    DefaultDepthOptions.WrapT = GL_CLAMP_TO_EDGE;

    return DefaultDepthOptions;
}
