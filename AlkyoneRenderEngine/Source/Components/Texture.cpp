#include <Components\Texture.h>
#include <Components/Image.h>

Texture::Texture()
{
    Width = 0;
    Height = 0;
    TextureID = 0;
    //Format = GL_RGB;

   

    Target = GL_TEXTURE_2D;
    InternalFormat = GL_RGB8;
    Format = GL_RGB;
    Type = GL_UNSIGNED_BYTE;
    FilterMin = GL_LINEAR_MIPMAP_LINEAR;
    FilterMag = GL_LINEAR;
    WrapS = GL_REPEAT;
    WrapT = GL_REPEAT;
    WrapR = GL_REPEAT;
    Mipmapping = GL_FALSE;
}

Texture::Texture(uint32 InWidth, uint32 InHeight): Width(InWidth), Height(InHeight), TextureImage()
{
    Target = GL_TEXTURE_2D;
    InternalFormat = GL_RGB8;
    Format = GL_RGB;
    Type = GL_UNSIGNED_BYTE;
    FilterMin = GL_LINEAR;
    FilterMag = GL_LINEAR;
    WrapS = GL_CLAMP_TO_EDGE;
    WrapT = GL_CLAMP_TO_EDGE;
    WrapR = GL_REPEAT;
    Mipmapping = GL_FALSE;
}

Texture::~Texture()
{
}

void Texture::Generate(std::string filename, bool IsHDRImage)
{
    glCreateTextures(Target, 1, &TextureID);

    glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, FilterMag);

    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, WrapS);
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, WrapT);

    TextureImage = new Image(filename.c_str(), true, IsHDRImage);

    Width = TextureImage->Width;
    Height = TextureImage->Height;

    if (IsHDRImage) {
        glTextureStorage2D(TextureID, 1, GL_RGB32F, Width, Height);
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureStorage2D(TextureID, 1, GL_RGB8, Width, Height);
        glTextureSubImage2D(TextureID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());
    }
    if (Mipmapping == GL_TRUE)
    {
        glGenerateTextureMipmap(TextureID);
    }
}

void Texture::GenerateTextureLayers(TArray<std::string> filenames, bool IsHDRImage)
{
    glCreateTextures(Target, 1, &TextureID);

    glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, FilterMag);

    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, WrapS);
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, WrapT);

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

void Texture::GenerateDepthTexture(bool IsForShadows) {
    glCreateTextures(Target, 1, &TextureID);

    if (IsForShadows) {
        glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(TextureID, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(TextureID, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else {

        glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, FilterMin);
        glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, FilterMag);
        glTextureParameteri(TextureID, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTextureParameteri(TextureID, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glTextureStorage2D(TextureID, 1, GL_DEPTH_COMPONENT32F, Width, Height);

    //TextureHandle = glGetTextureHandleARB(TextureID);
	CHECK_GL_ERROR();

}

void Texture::GenerateColourTexture() {
    glCreateTextures(Target, 1, &TextureID);

    glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, FilterMag);

    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, WrapS);
    glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, WrapT);

    glTextureStorage2D(TextureID, 1, GL_RGBA8, Width, Height);

}

void Texture::Bind(int32 Unit)
{
    if (Unit >= 0)
    {
        glActiveTexture(GL_TEXTURE0 + Unit);
    }
    glBindTexture(Target, TextureID);
}

void Texture::Unbind()
{
    glBindTexture(Target, 0);
}

void Texture::SetWrapModeS(uint32 WrapMode, uint8 ShouldBind)
{
    WrapS = WrapMode;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
        Unbind();
    }
}

void Texture::SetWrapModeT(uint32 WrapMode, uint8 ShouldBind)
{
    WrapT = WrapMode;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(Target, GL_TEXTURE_WRAP_T, WrapT);
        Unbind();
    }
}

void Texture::SetFilterMin(uint32 Filter, uint8 ShouldBind)
{
    FilterMin = Filter;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, FilterMin);
        Unbind();
    }
}

void Texture::SetFilterMag(uint32 Filter, uint8 ShouldBind)
{
    FilterMag = Filter;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, FilterMag);
        Unbind();
    }
}
