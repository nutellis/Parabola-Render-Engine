#pragma once

#include <Core/PODTypes.h>

template <typename T> class TArray;
class VertexFormat;
class Texture;

using VertexArray = TArray<VertexFormat>;
using IndexArray = TArray<uint32>; 
using TextureArray = TArray<Texture>;

