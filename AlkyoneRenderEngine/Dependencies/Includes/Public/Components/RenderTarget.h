#pragma once

#include <Core/RenderCore.h>

//A 'canvas' which can receive the results of a rendering operation. 


class RenderTarget
{
public:
	RenderTarget();

	~RenderTarget();


	 uint8 isActive() const ;


	 uint32 GetHeight(void) const;
	 uint32 GetWidth(void) const;
	 const char * getName(void) const;
	 void getMetrics(uint32 & OutWidth, uint32 & OutHeight, uint32 & OutColourDepth);

protected:

	const char * Name;
	uint32 ID;

	uint8 Active : 1;
	uint8 Priority;

	
	uint32 Width;
	uint32 Height;
	uint32 ColourDepth;

};






struct FrameStats
{
	float lastFPS;
	float avgFPS;
	float bestFPS;
	float worstFPS;
	unsigned long bestFrameTime;
	unsigned long worstFrameTime;
	size_t triangleCount;
	size_t batchCount;
};