#ifndef CORE_H
#define CORE_H



typedef signed long long int64;
typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned char uint8;
typedef char int8;

//typedef const char *  string;

class GLogManager;
class GRenderManager;
class GWindowManager;
class GShaderManager;


#include <memory>

#define TRUE  1
#define FALSE 0
//using std::unique_ptr;


#endif

//#include <Reflection\Types.h>
//#include <Object.h>
//#include <ObjectMacros.h>
//#include <iostream>

//
//#define OBJECTS PObjectRegister::getInstance()
//
//#define TYPES RClassRegister::getInstance()