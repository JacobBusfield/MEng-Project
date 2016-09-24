// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "route.h"
#include "node.h" // Forward Declaration

// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Default
Route::Route(){}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Add a node to be the next in the current list of nodes the car must visit.
void Route::addRoute(Node * n){
	status = NOT_STARTED;
	list.push_back(n);
}

// Returns the pointer for the next node required to be visited.
Node * Route::getRoute(void){
	if (!list.empty()) {
		return list[0];
	}
	return NULL;
}

// Removes the head route off the list of nodes to visit.
void Route::updateRoute(void){
	if (!list.empty()) {
		list.erase(list.begin());
	}
}

// Requires updating every frame! Updates how long car has been following a route for.
void Route::updateRouteClock(void)
{
	if (status == UNCOMPLETE) { clock.updateClock(); }
}

// If car has route, returns true.
bool Route::routeAvailable(void)
{
	if (list.empty()) { return false; }
	return true;
}

// If car is currently following a route, returns true.
bool Route::followingRoute()
{
	if ((status == NOT_STARTED) || (status == UNCOMPLETE)) { return true; }
	return false;
}

// Notifies Route class that car has started following its route.
void Route::routeStart()
{
	if (status == NOT_STARTED) { status = UNCOMPLETE; }
}

// Notifies Route class that car has finished following its route. 
void Route::routeDone()
{
	if (status != NONE) { status = COMPLETE; }
}

// Returns how long the route took to complete. If called before route is finished, returns -1.
double Route::routeDuration()
{
	if (status == COMPLETE) { return clock.getElapsedTime(); }
	return -1;
}

// -----------------------------------------------------------------------------------------------//

