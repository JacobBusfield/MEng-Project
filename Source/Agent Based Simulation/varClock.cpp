// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "varClock.h"
#include "config.h"

// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Default.
VarClock::VarClock(){}
VarClock::~VarClock(){}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Get the time that has passed = no. of frame passed * FRAME_TIME.
double VarClock::getElapsedTime(void){
	return time;
}

// Reset the stopwatch back to 0.
void VarClock::restart(void){
	time = 0;
}

// Add another time increment to the clock.
void VarClock::updateClock(void){
	time += (double)FRAME_TIME;
}

// -----------------------------------------------------------------------------------------------//
