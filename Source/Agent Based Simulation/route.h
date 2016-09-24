// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "varClock.h"
#include "SFML/Graphics.hpp"

// Forward Declaration
class Node; 

// Public Type Definition
enum RouteStatus { 
	NONE, 		// No route has been specified.
	NOT_STARTED,// A route exists but hasn't yet been started.
	UNCOMPLETE, // A route has been started, but hasn't been completed
	COMPLETE    // A route has been completed.
};

// Class: Route
//
// This class is responsible for storing a route between nodes in which a car can follow.
class Route
{
public:
// Constructor:
	// Default
	Route();

// Public Functions:
	// Add a node to be the next in the current list of nodes the car must visit.
	void  addRoute(Node * n);
	
	// Returns the pointer for the next node required to be visited.
	Node* getRoute(void);
	
	// Removes the head route off the list of nodes to visit.
	void  updateRoute(void);
	
	// Requires updating every frame! Updates how long car has been following a route for.
	void  updateRouteClock(void);
	
	// If car has route, returns true.
	bool  routeAvailable(void);
	
	// If car is currently following a route, returns true.
	bool  followingRoute();
	
	// Notifies Route class that car has started following its route.
	void  routeStart();
	
	// Notifies Route class that car has finished following its route. 
	void  routeDone();
	
	// Returns how long the route took to complete. If called before route is finished, returns -1.
	double routeDuration();

private:
// Private Variables:
	std::vector<Node*> list;	// List of nodes to visit in the route.
	RouteStatus status = NONE;  // Status of current route.
	VarClock clock;				// Tracks how long route has been actively followed for.
};

