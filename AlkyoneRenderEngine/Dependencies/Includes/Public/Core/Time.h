#pragma once


//Windows Time
class Time {

public:

	Time();
	~Time();

	static void InitTime();

	static double Seconds();

public: //private:

	//Frequency
	static double SecondsPerCycle;

	/** Holds time step if a fixed delta time is wanted. */
	static double FixedDeltaTime;

	/** Holds current time. */
	static double CurrentTime;

	/** Holds previous value of CurrentTime. */
	static double LastTime;

	/** Holds current delta time in seconds. */
	static double DeltaTime;

	/** Holds time we spent sleeping in UpdateTimeAndHandleMaxTickRate() if our frame time was smaller than one allowed by target FPS. */
	static double IdleTime;

	/** Holds the amount of IdleTime that was LONGER than we tried to sleep. The OS can't sleep the exact amount of time, so this measures that overshoot. */
	static double IdleTimeOvershoot;
};
