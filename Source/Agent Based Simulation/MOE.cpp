// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "MOE.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//
// Default constructor/ deconstructor
MOE::MOE(){}
MOE::~MOE(){}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Clear all stored car durations.
void MOE::resetMOE()
{
	carDurations.clear();
}

// Add car duration to list 'all' car durations.
void MOE::addCarDuration(double duration)
{
	return carDurations.push_back(duration);
}

// Get the list of all car durations.
std::vector<double> MOE::getCarDurations()
{
	return carDurations;
}

// Get the summed value of all car durations.
double MOE::getCombinedCarDuration()
{
	return std::accumulate(carDurations.begin(), carDurations.end(), 0.0);
}

// -----------------------------------------------------------------------------------------------//

