// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "node.h" // Forward declaration.
#include "road.h" // Forward declaration.
#include <iostream>
using namespace std;


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Constructs road object, and sets speed limit of road.
Road::Road(const float speedLimit) : color(sf::Color(90, 90, 90)), thickness(ROAD_THICKNESS) {
	speed = speedLimit/50;
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Connects two node, n1 n2, together using a straight road between both point.
void Road::connect(Node * n1, Node * n2){
	// Draw Road 
	//   Get the two node positions. Draw a rectangle around these points.
	a.pos = n1->getPosition();
	b.pos = n2->getPosition();
	//   Get vector between nodes.
	sf::Vector2f dir = a.pos - b.pos;
	sf::Vector2f unitDir = dir / std::sqrt(dir.x*dir.x + dir.y*dir.y);
	//   Get perpendicular vector, use it to get perpendicular vector of half the thickness.
	sf::Vector2f unitPerp(-unitDir.y, unitDir.x);
	sf::Vector2f offset = (thickness / 2.f)*unitPerp;
	//   Create four vertices that make the rectangle using perpendicular offset, and colour them.
	vertices[0].position = a.pos + offset;
	vertices[1].position = b.pos + offset;
	vertices[2].position = b.pos - offset;
	vertices[3].position = a.pos - offset;
	for (int i = 0; i < 4; ++i) { vertices[i].color = color; }

	// Make Connections 
	a.connection = n1;
	a.altConnection = n2;
	b.connection = n2;
	b.altConnection = n1;
	n1->connect(this);
	n2->connect(this);

	// Path Calculations
	a.path = b.pos - a.pos;
	a.unitPath = a.path / std::sqrt(a.path.x*a.path.x + a.path.y*a.path.y);
	b.path = a.pos - b.pos;
	b.unitPath = b.path / std::sqrt(b.path.x*b.path.x + b.path.y*b.path.y);

	// Traffic Lights
	a.lights = Lights(a.pos.x + a.unitPath.x * 50, a.pos.y + a.unitPath.y * 50, a.unitPath);
	b.lights = Lights(b.pos.x + b.unitPath.x * 50, b.pos.y + b.unitPath.y * 50, b.unitPath);
}

// Draw the road onto the SFML render window.
void Road::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.draw(vertices, 4, sf::Quads);
	target.draw(a.lights);
	target.draw(b.lights);
}

// Updates cars within the road (handles grabbing, and passing, them to connected nodes).
void Road::update(void){
	// Process conducted for both sides of the road.
	updateQueue(&a);
	updateQueue(&b);
}

// Add car to road, coordinates passed to this function determine which side is connected to the
// object wanting to add cars.
void Road::add(Car * newCar, sf::Vector2f side) {
	// Car's speed limited to road's speed limit.
	newCar->setMaxSpeed(speed);

	if (side == a.pos) {
		addCarStart(newCar, &a);
		addCarEnd(newCar, &b);
	}
	else {
		addCarStart(newCar, &b);
		addCarEnd(newCar, &a);
	}
}

// Returns position of the road end opposite to the one given by the coords passed in.
sf::Vector2f Road::getOtherSidePosition(sf::Vector2f pos){
	if (pos == a.pos) { return b.pos; }
	return a.pos;
}

// Returns Node pointer for the road end opposite to the one given by the coords passed in.
Node * Road::getOtherSideConnection(sf::Vector2f pos){
	if (pos == a.pos) { return b.connection; }
	return a.connection;
}

// Returns tail car leaving from a position, returns null if none exist.
Car * Road::getCarLeaving(sf::Vector2f pos)
{
	if (pos == a.pos) {
		if (!a.car.empty()) { return a.car.back(); }
		return NULL;
	}
	else if (!b.car.empty()) { return b.car.back(); }
	return NULL;
}

// Return head car entering a position, return null if none exist. 
Car * Road::getCarEntering(sf::Vector2f pos)
{
	if (pos == a.pos) {
		if (!b.car.empty()) { return b.car[0]; }
		return NULL;
	}
	else if (!a.car.empty()) { return a.car[0]; }
	return NULL;
}

// Return true if car is waiting at end of the road (waiting on red light).
bool Road::carWaiting(sf::Vector2f pos)
{
	if (pos == a.pos) {
		if (!b.car.empty()) {
			if (b.car[0]->getPosition() == b.car[0]->getDestination()) { return true; }
		}
		return false;
	}
	else if (!a.car.empty()) {
		if (a.car[0]->getPosition() == a.car[0]->getDestination()) { return true; }
	}
	return false;
}

// Return true if the end of the road is stopped (red lights).
bool Road::stopped(sf::Vector2f pos){
	if (pos == a.pos) {
		if (b.state == STOP) { return true; }
	}
	else if (a.state == STOP) { return true; }
	return false;
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PRIVATE FUNCTIONS                                       //
// -----------------------------------------------------------------------------------------------//

// Updates Cars on single side of a road (cars heading away from roadEnd specified).
void Road::updateQueue(RoadEnd * r) {
	// Update Local Clocks from Highest Level
	clock.updateClock();

	// Update state (lights) for road in both directions.
	updateState(r);

	int i;
	for (i = 0; i < (int)r->car.size(); i++) {
		// For each head car, cars where there isnt a car ahead (leader may have just been removed),
		// move the car with respect to it's destination (the road end). Once there, the inside of 
		// the following clause will be true, if can be added to the connection do so. The next car
		// will be then investigated as the new head (popping out to the for loop). 
		if (r->car[i]->drive(r->car[i]->getDestination())) {                                
			if (r->state == GO) {
				if (r->altConnection->add(r->car[i])) {
					// Fixes bug where cars may clip each other, if car behind is travelling faster 
					// than head when head is released - by setting cars to same speed.
					if (r->car.size() > 1) {
						if (r->car[i]->getSpeed() < r->car[i + 1]->getSpeed()) {
							if (!r->car[i + 1]->canSpawn(100, r->car[i]->getPosition())) {
								r->car[i + 1]->setSpeed(r->car[i]->getSpeed());
							}
						}
					}
					r->car.erase(r->car.begin());
				}
				else { r->car[i]->setSpeed(0); }
			}
			else { r->car[i]->setSpeed(0); }
		}
		else {
			// When there is a car infront, move cars with respect to the car infront. For each a 
			// desired serpation is calulated and this compared to the actual seperation. If the car
			// could drive forwards (Not Back) to improve this, it does so. Note that the +2 in this
			// eqaulity is to handle undefined behaviour (rounding errors on floating point) and to 
			// add natural behaviour (response lag). Once the destination has been reached the car's
			// speed reduces to zero.
			for (i = 1; i < (int)r->car.size(); i++) {
				float desiredSep = 
					r->car[i]->getScale().x * 0.5f *(r->car[i]->getLocalBounds().width + 
					r->car[i-1]->getLocalBounds().width)+2.f*r->car[i-1]->getSpeed()+MIN_SEPERATION;
					
				sf::Vector2f sepV = (r->car[i - 1]->getPosition() - r->car[i]->getPosition());
				float seperation = std::sqrt((sepV.x * sepV.x) + (sepV.y * sepV.y));
				if (desiredSep < seperation+2) { 
					sf::Vector2f antiPath(desiredSep * r->unitPath.x, desiredSep * r->unitPath.y);
					if (r->car[i]->drive(r->car[i - 1]->getPosition() - antiPath)) {
						r->car[i]->setSpeed(0);
					}
				}
			}
			// May as well break - checking outer-for-loop wastes CPU time (never true).
			break;
		}
	}

	// If road isnt clear, check car at the end of the queue doesn't overlap road start. If not,
	// add car from connection if able. Else if the road is clear can just add car straight away.
	if (!r->car.empty()) {
		if (r->car.back()->canSpawn(r->connection->getRadius(), r->pos)) {
			Car* temp = r->connection->get(this);
			if (temp != NULL) {
				add(temp, r->pos);
			}
		}
	}
	else {
		Car* temp = r->connection->get(this);                                                                     
		if (temp != NULL) {
			add(temp, r->pos);
		}
	}
}

// Adds where the car should start (Road starts in Node)
void Road::addCarStart(Car * newCar, RoadEnd * r){
	newCar->setPosition(r->pos);
	r->car.push_back(newCar);
}

// Adds where the car should stop (before the connecting node) 
void Road::addCarEnd(Car * newCar, RoadEnd * r){
	sf::Vector2f room((r->connection->getRadius() + 
					  (newCar->getScale().x * newCar->getLocalBounds().width) / 2) * r->unitPath.x
					  ,
		              (r->connection->getRadius() + 
					  (newCar->getScale().x * newCar->getLocalBounds().width) / 2) * r->unitPath.y);
	newCar->setDestination(r->pos + room);
}

// Updates state of lights for road end. This includes transition time/ states between STOP and GO
// states.
void Road::updateState(RoadEnd * r){
	// Uses Finite State Machine:
	//
	//				  time                     connection active
	//					------- (STOP) --->---
	//					/\					   V
	//					(PREPARE TO STOP)	   (PREPARE TO GO)
	//					/\					   V
	//					----<--- (GO) ----<---
	// connection unactive					   time			      
	
	if (r->altConnection->checkConnection(this)) {
		if (r->state == STOP) {
			r->state = PREPARE_TO_GO;
			clock.restart();
		}
		else if (clock.getElapsedTime() > 0 + TIME_PTG) { r->state = GO; }
	}
	else {
		if (r->state == GO) {
			r->state = PREPARE_TO_STOP;
			clock.restart();
		}
		else if (clock.getElapsedTime() > 0 + TIME_PTS) { r->state = STOP; }
	}
	updateLights(r);
}

// For specified road end, update the lights depending on state of the road end.
void Road::updateLights(RoadEnd * r){
	if (r == &a) { b.lights.update(r->state, b.connection->getWaitDuration(this)); }
	else { a.lights.update(r->state, a.connection->getWaitDuration(this)); }
}

// -----------------------------------------------------------------------------------------------//

