// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "node.h"
#include "config.h"
#include "SFML/Graphics.hpp"

// Forward declarations.
class Road; 
class Car;


// Class: Location
//
// This class is responsible for spawning and removing cars (defined in the Car.h) from the
// simulation (defined in Simulation.h). Spawned cars are added (using add()) to roads (defined in
// Road.h) and cars incoming from roads (from get()) are the ones removed.
//
// Inherits:
//   Node - a virtual class used to connect roads together. By inheriting this it mean locations and
//          systems (in system.h) can be treat identically when required.
class Location : public Node
{
public:
// Constructor:
	// Creates a location at the coordinated given by p.
	Location(const sf::Vector2f p);

// Public Functions:
	// Updates location state, and cars connected to it
	void update(void);
	
	// Adds car to list of cars to be spawned 
	void spawn(Car * newCar);

	// Adds car to location periodically if location is empty 
	bool add(Car * newCar);
	
	// Spawns a car periodically if location is empty. Handles starting car's route as well.
	Car* get(Road * n);

	// Connect road to the location 
	void connect(Road* r);
	
	// Returns true if live connection is requested, false otherwise 
	bool checkConnection(Road * r);

	// Unused: Neede to satisfy virtual Node class.
	double getWaitDuration(Road * r);

private:
// Private Functions:
	// Cycles live connection intertively.
	void changeLiveConnection(void);
	
// Private Type Definition
	typedef struct {
		std::vector<Road*> list; // List of connected roads
		int no = 0;              // Number of connected roads (saves computational time).
		int live = 0;			 // Index of the road currently 'live' (green).
		int rand = 0;			 // Random road is chosen by incrementing through connected road.
		                         //   This varibale tracks that progression.
	} Connection;

// Private Variables:	
	Connection c; 				 // List of connected roads
	std::vector<Car*> carOut; 	 // List of cars needing to leave location (spawn list).
	std::vector<Car*> carIn;  	 // List of cars needing to enter location.
	VarClock clock, spawnClock;  // clocks - handling state, and when to spawn a car respectively.
};


