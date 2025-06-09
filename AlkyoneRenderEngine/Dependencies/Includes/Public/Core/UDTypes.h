#pragma once

#include <Core/PODTypes.h>

template <typename T> class TArray;
class RTexture;
struct VertexFormat;

using VertexArray = TArray<VertexFormat>;
using IndexArray = TArray<uint32>; 
using TextureArray = TArray<RTexture>;

