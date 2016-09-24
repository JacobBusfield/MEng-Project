// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "System.h"
#include "road.h"      // Forward declaration.
#include "varClock.h"
#include <iostream>
using namespace std;


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Create a round-robin system.
System::System(const sf::Vector2f p){
	init(p);
	setOutlineColor(sf::Color::White);
}

// Create a system that has, at least in part, an active controller.
System::System(const sf::Vector2f p, std::vector<double*> in, std::vector<double*> out) {
	init(p);
	addController(in, out);
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Adds controller to a system. This can be conducted at any time.
void System::addController(std::vector<double*> in, std::vector<double*> out)
{
	// IO from network assigned. 
	//  - Assigning too many IOs will leave some network IOs unconnected.
	//	- Assigning too few  IOs will leave some system  IOs unconnected.
	//  - Repeat Ins are handled correctly, but repeat outs will result in former ones being 
	//	  ignored (overwritten).
	controller.in = in;
	controller.out = out;
	//
	// Expecting In[1] - In[n] to be lane next to 'GO'. Network should have 'memory' to pick next
	// lane naturally.
	// Expecting out[1] - out[n] to be each connection and whether car is present on it.

	// non-round-robin controlled junctions are set to be green.
	setOutlineColor(sf::Color::Green);
}

// Updates system. This includes: which road is set to active, moving cars through the system and 
// updating the outputs of the controller (inputs of epiNet for instance).
void System::update(void){
	// --- Update Local Clocks from Highest Level --- //
	clock.updateClock();

	// --- Update Lights --- //
	if (c.waiting == true) {
		// c.waiting is set, if the system is waiting for a road to change to 'STOP'.
		if (c.list[c.live]->stopped(getPosition())) {
			c.waiting = false;
			clock.restart();
		}
	}
	else if (clock.getElapsedTime() > TIME_GO) {
		changeLiveConnection();
		clock.restart();
	}

	// --- Update Car --- //
	if (!driving.empty() && ready.empty()) {
		if (state == ENTERING) {
			// The car is just entering system, when it reaches the centre the condition will be
			// true.
			if (driving[0]->drive(getPosition())) {
				// Trying to work out the angle 'toTurn' car and in which direction. First get the 
				// current road the car is to drive onto. Then the direction the car must face is
				// calculated and set to 180. This leaves an offset 'dif' which the 'carAngle' is 
				// normalised to. The 'carAngle' is used to determine the direction; 'toTurn' the 
				// angle to turn.
				sf::Vector2f rPos = c.list[c.rand]->getOtherSidePosition(getPosition());
				for (unsigned int i = 0; i < c.no; i++) {
					if (driving[0]->getRoute() == c.list[i]->getOtherSideConnection(getPosition())) {
						rPos = c.list[i]->getOtherSidePosition(getPosition());
						break;
					}
				}
				float dif = (((atan2(rPos.y - getPosition().y, rPos.x - getPosition().x)) * 180)
							  / 3.14159265f) - 180;
				float carAngle = (float)fmod(driving[0]->getRotation()+360 - dif, 360);
				toTurn = sqrt((carAngle - 180)*(carAngle - 180));
				if (carAngle > 180) { state = LEFT; } 
				else { state = RIGHT; }
			}
		}
		else if ((state == LEFT)||(state == RIGHT)){
			// The car is in the centre and needs to turn according to its 'state' and the angle 
			// 'toTurn'. If finished turning, add to cars that are 'ready' to be grabbed. The car is
			// still tracked by 'driving' until it leaves the location to prevent another car 
			// entering. If the car still needs to turn on the other hand, turn it by a fixed amount
			// in the correct direction.
			if (toTurn <= 0) {
				ready.push_back(driving[0]);
				state = LEAVING;
			}
			else {
				if (state == LEFT) { driving[0]->rotate(-TURN_RATE); }
				else { driving[0]->rotate(TURN_RATE); }
				toTurn -= TURN_RATE;
			}
		}
		else { // STATE == LEAVING
			// The car has been given to a road to move, so it is removed from the system.
			state = ENTERING;
			driving.erase(driving.begin());
		}
	}

	// --- UPDATE OUTPUTS --- //
	// out[1 to n] is lanes with cars on them...
	for (unsigned int i = 0; i < c.list.size(); i++) {
		if (i < controller.out.size()) {
			if (c.list[i]->carWaiting(getPosition())) {
				// ... If car just waiting, output = 0.5. start with strong value to 'kick' epi in 
				// to action ...
				if (*controller.out[i] < 0.5) {                                         
					*controller.out[i] = 0.5;
				}
				else if (*controller.out[i] < (1-framesInc)) {
					 // ... Value increments over time to max of 1
					*controller.out[i] += framesInc;                                    
				}
			}
			 // Else if no car output = 0.
			else *controller.out[i] = 0;                                                
		}
	}
}

// Add car to the system. Returns false if unable to add the car. Function that calls this function
// should therefore stop attempting this function when the function returns true.
bool System::add(Car * newCar){
	if (driving.empty()) {
		driving.push_back(newCar);
		return true;
	}
	return false;
}

// Get car from system. Returns NULL if unable to get car. 
Car * System::get(Road * n){
	if (!ready.empty()) {
		// If car has route, only let road (road.h) grab it if it can take it where it needs to go.
		if (ready[0]->routeAvailable()) {
			if (n->getOtherSideConnection(getPosition()) == ready[0]->getRoute()) {
				ready[0]->updateRoute();
				Car * temp = ready[0];
				ready.erase(ready.begin());
				return temp;
			}
		}
		// Else if no route, allow next random (itertive cycle) road take car.
		else if (n == c.list[c.rand]) {
			if (c.rand >= c.no - 1) {
				c.rand = 0;
			}
			else { c.rand++; }
			Car * temp = ready[0];
			ready.erase(ready.begin());
			return temp;
		}
	}
	return NULL;
}

// Connect a road to this system. This adds the road pointer to this system. The system will access 
// functions from that road class to pass cars as required.
void System::connect(Road * r) {
	c.list.push_back(r);
	c.no++;
}

// Checks if the road, passed in, has been previously connected to the system.
bool System::checkConnection(Road * r){
	if (c.waiting == true) { return false; }
	else if (r == c.list[c.live]) { return true; }
	return false;
}

// Checks all system connections for the requested road. If match returns how long a car has been 
// waiting at the connection.
double System::getWaitDuration(Road * r)
{
	for (unsigned int i = 0; i < c.no; i++) {
		if (r == c.list[i]) {
			if ((int)controller.out.size() > i) { return *controller.out[i]; }
		}
	}
	return 0;
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PRIVATE FUNCTIONS                                       //
// -----------------------------------------------------------------------------------------------//

// Change which road is currently active. If a controller is present it uses the outputs from it
// (binary encoded) to work out next active road, else uses round-robin configuration.
void System::changeLiveConnection(void){
	if (c.list.size() > 1) {
		// convert outputs to binary. <0.5 = 0, >0.5 =1
		if (c.no <= pow(2, controller.in.size())) {
			unsigned int count = (unsigned int)ceil(log2(c.no));
			unsigned int temp = 0;

			for (unsigned int i = 0; i < count; i++) {
				// dont slide on first time.
				if (i > 0) { temp = temp << 1; }
				if (*controller.in[i] > 0.5) { temp++; }
			}
			c.live = temp;
			
			// Protects aginst case where need only odd numbers decoded from binary.
			if (c.live >= c.no) { c.live = c.no - 1; }
		}
		// if no controller, increase live connection each turn.
		else {
			c.store++;
			if (c.store > c.no - 1) { c.store = 0; }
			c.live = c.store;
		}
	}
}

// -----------------------------------------------------------------------------------------------//