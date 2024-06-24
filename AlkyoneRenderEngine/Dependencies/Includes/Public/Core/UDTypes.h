#pragma once

#include <Core/PODTypes.h>

template <typename T> class TArray;
class VertexFormat;
class Texture;


//class Channel;

//class PMaterial;


using VertexArray = TArray<VertexFormat>;
using IndexArray = TArray<uint32>; 
using TextureArray = TArray<Texture>;

