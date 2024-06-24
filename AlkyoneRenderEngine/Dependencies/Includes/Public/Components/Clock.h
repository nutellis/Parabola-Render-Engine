#pragma once

#include <Core/RenderCore.h>

#include <chrono>

class Clock
{
	static uint64 CyclesPerSecond;

	static void Init();
};