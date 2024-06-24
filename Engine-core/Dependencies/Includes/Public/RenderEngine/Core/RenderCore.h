#pragma once

#include <GL/glew.h>

#if defined(_MSC_VER)
typedef    __int64    int64;
#else
typedef signed long long int64;
#endif

typedef int64 intptr;

typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned char uint8;
typedef char int8;


class GLogManager;

class GRenderManager;
class GWindowManager;
class GShaderManager;


#include <memory>

#define TRUE  1
#define FALSE 0