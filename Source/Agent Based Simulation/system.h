// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "car.h"
#include "node.h"
#include "route.h"
#include "config.h"
#include "SFML/Graphics.hpp"

// Forward declaration.
class Road; 


// Class: System
//
// This class is responsible for transporting cars (defined in the Car.h) between roads (defined in 
// road.h). Systems have traffic lights which only allow a single road to pass their traffic through
// them at any one time. Systems can be controlled by a traffic controller, else they assume a 
// round-robin configuration.
//
// Inherits:
//   Node - a virtual class used to connect roads together. By inheriting this it mean locations and
//          systems can be treat identically when required.
class System : public Node
{
public:
// Constructor:
	// Create a round-robin system.
	System(const sf::Vector2f p);
	// Create a system that has, at least in part, an active controller.
	System(const sf::Vector2f p, std::vector<double*> in, std::vector<double*> out);
	
	
// Public Functions:
	// Adds controller to a system. This can be conducted at any time.
	void addController(std::vector<double*> in, std::vector<double*> out);

	// Updates system. This includes: which road is set to active, moving cars through the system
	// and updating the outputs of the controller (inputs of epiNet for instance).
	void update(void);

	// Add car to the system. Returns false if unable to add the car. Function that calls this
	// function should therefore stop attempting this function when the function returns true.
	bool add(Car * newCar);
	
	// Get car from system. Returns NULL if unable to get car. 
	Car* get(Road * n);
	
	// Connect a road to this system. This adds the road pointer to this system. The system will 
	// access functions from that road class to pass cars as required.
	void connect(Road* r);
	
	// Checks if the road, passed in, has been previously connected to the system.
	bool checkConnection(Road * r);
	
	// Checks all system connections for the requested road. If match returns how long a car has
	// been waiting at the connection.
	double getWaitDuration(Road * r);

private:
// Private type declaration:
	
	typedef struct { // Struct tracking inputs, and outputs of system used by external controller.
		std::vector<double*> in;  // Controller inputs (controller instructions).
		std::vector<double*> out; // Controller outputs (road state).
	} Controller;
	
		
	typedef struct { // Struct tracking variables related to the connections to the system.
		std::vector<Road*> list;	// Set of roads connected to this system. 
		unsigned int no = 0;     	// Number of connections (value stored to save CPU time).
		unsigned int live = 0;  	// Which road to get cars from.
		unsigned int store = 0;		// saves next live road when controller is present.
		bool waiting = false; 		// Determines when live connection has transitioned between 
									// STOP and GO states. Needed so another road cannot be set
									// to active while a road is transitioning to stop for instance.
		unsigned int rand = 0;   	// which road to randomly send cars to (cycles circularly).
	} Connection;

	enum CarState {
		ENTERING, // Car is entering the system, not yet reached the centre. 
		LEFT,     // Car has reached centre and is turning left.
		RIGHT,    // Car has reached centre and is turning right.
		LEAVING   // Car is leaving system in pursuit of joining a road.
	};
	
// Private Functions:
	// Change which road is currently active. If a controller is present it uses the outputs from it
	// (binary encoded) to work out next active road, else uses round-robin configuration.
	void changeLiveConnection(void);

// Private Variables:
	CarState state = ENTERING; // State of car currently in the system.
	std::vector<Car*> driving; // Car currently in the system (Vector handles single Car well)
	std::vector<Car*> ready;   // Cars that can be 'grabbed' (using get()) from the system
	Connection c;			   // All the information about the system's road connections.
	Controller controller;     // All the information the controller needs to change lights.
	float toTurn;              // Angle to track how much a car needs to turn
	VarClock clock;			   // Clock that determines the minimum rate at which lights can change.
							   // Required to meet UK traffic control laws.
	
	// frames to run to fill progress bar. Stored in memory to save CPU time.
	const double framesInc = (double)(0.5 / (TIMEOUT_WAITING_CAR_INPUT / FRAME_TIME));
};


