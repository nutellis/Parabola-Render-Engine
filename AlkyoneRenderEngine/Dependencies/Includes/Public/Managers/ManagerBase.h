#pragma once

#include <Core/RenderCore.h>

#define INIT_OK 1


class ManagerBase{
	
public:
	virtual void Init() = 0;
	virtual void Terminate() =0;

	uint8 bInitSuccessful : 1;
};