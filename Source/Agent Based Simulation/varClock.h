// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once

// Class: 
//
// Class used to track time elapsed in the simulation. Each increment is defined as the FRAME_TIME
// in config.h. Each timer must be manually updated using updateClock() in order to keep the correct
// time.
class VarClock
{
public:
// Constructor:
	// Default.
	VarClock();
	~VarClock();

	
// Public Functions:
	// Get the time that has passed = no. of frame passed * FRAME_TIME.
	double getElapsedTime(void);
	
	// Reset the stopwatch back to 0.
	void restart(void);
	
	// Add another time increment to the clock.
	void updateClock(void);

private:
// Private Variables:
	double time = 0;	// tracks how much time has passed.
};

