#include <Components\Texture.h>
#include <Components/Image.h>

Texture::Texture()
{
    Width = 0;
    Height = 0;
    //Format = GL_RGB;

    Target = GL_TEXTURE_2D;
    InternalFormat = GL_RGB8;
    Format = GL_RGB;
    Type = GL_UNSIGNED_BYTE;
    FilterMin = GL_LINEAR_MIPMAP_LINEAR;
    FilterMax = GL_LINEAR;
    WrapS = GL_REPEAT;
    WrapT = GL_REPEAT;
    WrapR = GL_REPEAT;
    Mipmapping = GL_FALSE;
}

Texture::~Texture()
{
}

void Texture::Generate(std::string filename, bool IsHDRImage)
{
    glCreateTextures(Target, 1, &textureID);

    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, FilterMax);

    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, WrapS);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, WrapT);

    TextureImage = new Image(filename.c_str(), true, IsHDRImage);

    Width = TextureImage->Width;
    Height = TextureImage->Height;

    if (IsHDRImage) {
        glTextureStorage2D(textureID, 1, GL_RGB32F, Width, Height);
        glTextureSubImage2D(textureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureStorage2D(textureID, 1, GL_RGB8, Width, Height);
        glTextureSubImage2D(textureID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());
    }
    if (Mipmapping == GL_TRUE)
    {
        glGenerateTextureMipmap(textureID);
    }
}

void Texture::GenerateMipMaps(TArray<std::string> filenames, bool IsHDRImage)
{
    glCreateTextures(Target, 1, &textureID);

    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, FilterMin);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, FilterMax);

    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, WrapS);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, WrapT);

    TextureImage = new Image(filenames[0].c_str(), true, IsHDRImage);

    Width = TextureImage->Width;
    Height = TextureImage->Height;

    if (IsHDRImage) {
        glTextureStorage2D(textureID, filenames.Size(), GL_RGB32F, Width, Height);
        glTextureSubImage2D(textureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureStorage2D(textureID, filenames.Size(), GL_RGB8, Width, Height);
        glTextureSubImage2D(textureID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());
    }
    glGenerateTextureMipmap(textureID);

    if (IsHDRImage) {
        glTextureSubImage2D(textureID, 0, 0, 0, Width, Height, GL_RGB, GL_FLOAT, TextureImage->GetDataFloat());
    }
    else {
        glTextureSubImage2D(textureID, 0, 0, 0, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->GetDataUint8());

    }

    for (int i = 1; i < filenames.Size(); i++)
    {
        Image image(filenames[i].c_str(), true, IsHDRImage);
        
        if (IsHDRImage) {
            glTextureSubImage2D(textureID, i, 0, 0, image.Width, image.Height, GL_RGB, GL_FLOAT, image.GetDataFloat());
        }
        else {
            glTextureSubImage2D(textureID, i, 0, 0, image.Width, image.Height, GL_RGB, GL_UNSIGNED_BYTE, image.GetDataUint8());
        }

    }
}

void Texture::Bind(int32 Unit)
{
    if (Unit >= 0)
    {
        glActiveTexture(GL_TEXTURE0 + Unit);
    }
    glBindTexture(Target, textureID);
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

void Texture::SetFilterMax(uint32 Filter, uint8 ShouldBind)
{
    FilterMax = Filter;
    if (ShouldBind)
    {
        Bind();
        glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, FilterMax);
        Unbind();
    }
}
