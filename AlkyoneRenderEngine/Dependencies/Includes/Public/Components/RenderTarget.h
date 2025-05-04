#pragma once

#include <Core/RenderCore.h>
#include <string>

//A 'canvas' which can receive the results of a rendering operation. 


class RenderTarget
{
public:
	RenderTarget();
	RenderTarget(std::string Name, uint32 inWidth, uint32 inHeight);

	~RenderTarget();


	 uint8 isActive() const ;


	 uint32 GetHeight(void) const;
	 uint32 GetWidth(void) const;
	 std::string getName(void) const;
	 void getMetrics(uint32 & OutWidth, uint32 & OutHeight, uint32 & OutColourDepth);


public:
	std::string Name;
	uint32 ID;
	uint32 Width;
	uint32 Height;

protected:

	

	uint8 Active : 1;
	uint8 Priority;

	

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