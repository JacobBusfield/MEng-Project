// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "location.h"
#include "car.h"		// Forward declarations.
#include "road.h"		// Forward declarations.	
#include "varClock.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Creates a location at the coordinated given by p.
Location::Location(const sf::Vector2f p){
	init(p);
	setOutlineColor(sf::Color::Blue);
	
	spawnClock.restart();
	clock.restart();
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Updates location state, and cars connected to it
void Location::update(void){
	// Update Local Clocks from Highest Level
	clock.updateClock();
	spawnClock.updateClock();

	// Update Lights
	if (clock.getElapsedTime() > TIME_GO) {
		changeLiveConnection();
		clock.restart();
	}

	// Drive cars to center
	if (!carIn.empty()) {
		if (carIn[0]->drive(getPosition())) {		
			carIn[0]->setDone();
			carIn.erase(carIn.begin());
		}
	}
}

// Adds car to list of cars to be spawned 
void Location::spawn(Car * newCar){
	carOut.push_back(newCar);
}

// Adds car to location periodically if location is empty 
bool Location::add(Car * newCar){
	if (carIn.empty()) {
		carIn.push_back(newCar);
		spawnClock.restart();
		return 1;
	}
	return 0;
}

// Spawns a car periodically if location is empty. Handles starting car's route as well.
Car * Location::get(Road * n){
	// c.live < 0 is reserved for turning all light off (not currently used).
	if (c.live < 0) {}
	// Cars are only spawned, and hence gotten using this function, every so often. A spawn clock is
	// used to regulate this.
	else if (spawnClock.getElapsedTime() > SPAWN_RATE) {
		if (!c.list[c.live]->carWaiting(getPosition())){                
			if (!carOut.empty()) {
				// A delay has been requested (NULL entry), oblige it.
				if (carOut[0] == NULL) {
					carOut.erase(carOut.begin());
					spawnClock.restart();
				}
				// If next car to be spawned has a route, only let it leave on a road that leads to
				// that route.
				else if (carOut[0]->routeAvailable()) {
					if (n->getOtherSideConnection(getPosition()) == carOut[0]->getRoute()) {
						carOut[0]->updateRoute();

						// Route is started, starts tracking how long car has been 'alive'.
						carOut[0]->routeStart(); 
						
						// Car is removed, spawn clocks reset etc.
						Car * temp = carOut[0];
						carOut.erase(carOut.begin());
						spawnClock.restart();
						clock.restart();
						return temp;
					}
				}
				// Else if not route, a 'random' road is chosen for the car. Wait for that road to
				// be the connected one.
				else if (n == c.list[c.rand]) {
					// Random road is chosen by cycling through road connections.
					if (c.rand >= c.no - 1) {
						c.rand = 0;
					}
					else { c.rand++; }

					// Route is started, starts tracking how long car has been 'alive'.
					carOut[0]->routeStart();
					
					// Car is removed, spawn clocks reset etc.
					Car * temp = carOut[0];
					carOut.erase(carOut.begin());
					spawnClock.restart();
					c.live = c.rand;
					clock.restart();
					return temp;
				}
			}
		}
	}
	return NULL;
}

// Connect road to the location 
void Location::connect(Road * r){
	c.list.push_back(r);
	c.no++;
}

// Returns true if live connection is requested, false otherwise 
bool Location::checkConnection(Road * r){
	if (c.live >= 0) {
		if (r == c.list[c.live]) {
			return true;
		}
	}
	return false;
}

// Cycles live connection intertively.
void Location::changeLiveConnection(void) {
	if (c.live >= c.no - 1) {
		c.live = 0;
	}
	else { c.live++; }
}

// UNUSED: to satisfy node class inheritance.
double Location::getWaitDuration(Road * r){ return 0.0; }
// -----------------------------------------------------------------------------------------------//