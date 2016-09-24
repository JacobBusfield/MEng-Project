// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include <vector>
#include <numeric>

// Class: MOE
//
// MOE stands for measures of effectiveness. This class is responsible for storing information about
// how well the simulation (in Simalation.h) has performed.
class MOE
{
public:
// Constructor:
	// Default.
	MOE();
	~MOE();

// Public Functions:
	// Clear all stored car durations.
	void resetMOE();
	
	// Add car duration to list 'all' car durations.
	void addCarDuration(double duration);
	
	// Get the list of all car durations.
	std::vector<double> getCarDurations();
	
	// Get the summed value of all car durations.
	double getCombinedCarDuration();

private:
// Private Variables:
	std::vector<double> carDurations; // List of all car durations in the simulation. Should be
	                                  // filled as the simulation progresses.
};

