#include<Core/Time.h>


#include <Windows.h>



/* Static initializations
 *****************************************************************************/

double Time::SecondsPerCycle = 0.0;

/** Holds time step if a fixed delta time is wanted. */
double Time::FixedDeltaTime = 1 / 60.0;

/** Holds current time. */
double Time::CurrentTime = 0.0;

/** Holds previous value of CurrentTime. */
double Time::LastTime = 0.0;

/** Holds current delta time in seconds. */
double Time::DeltaTime = 1 / 60.0;

/** Holds time we spent sleeping in UpdateTimeAndHandleMaxTickRate() if our frame time was smaller than one allowed by target FPS. */
double Time::IdleTime = 0.0;

/** Holds the amount of IdleTime that was LONGER than we tried to sleep. The OS can't sleep the exact amount of time, so this measures that overshoot. */
double Time::IdleTimeOvershoot = 0.0;

Time::Time()
{
}

Time::~Time()
{
}

void Time::InitTime()
{
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	
	SecondsPerCycle = 1.0 / Frequency.QuadPart;
}


//reasoning https://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/
double Time::Seconds()
{
	LARGE_INTEGER Cycles;
	QueryPerformanceCounter(&Cycles);

	// add big number to make bugs apparent where return value is being passed to float
	return Cycles.QuadPart * SecondsPerCycle + 16777216.0;

}
