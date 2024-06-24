#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H


#include <Core.h>

#include <Reflection\TypeRegister.h>
#include <Globals\ObjectRegister.h>


#include <iostream>
#include <unordered_map>



struct GArrays {
public:

	PObjectRegister ObjectRegister;
	RClassRegister ClassRegister;

	GArrays();


	PObject *Find(const char*);
	//uint8 Add(PObject *newObject);

private:
	



};



#endif // !_GLOBALS_H
