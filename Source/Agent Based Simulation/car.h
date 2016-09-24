// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "route.h"
#include "config.h"

// Forward declarations.
class Location; 
class Road;


// Class: Car
//
// A single car. The agent to be used by the Simulation class. It has limited functionality as to 
// remain computationally inexpensive; it can accelerate to a position and stop. It can also use 
// function from what it...
// 
// Inherits:
//   Sprite - SFML library class. An image with its own position and transformation functions.
//   Route  - Used to hold a set of Nodes that a car is required to visit. 
class Car: public sf::Sprite, public Route {	
public:
// Constructor:
	// Creates a car using the texture passed to it.
	Car(sf::Texture * texture);

	
// Public Functions:
	// Drives car to coordinates given by destination. Car stops, and return true when complete.
	bool drive(sf::Vector2f destination);
	
	// Determines if there is room between the current car's position and the 'point' + some 'space'
	// for a car to be created.
	bool canSpawn(float space, sf::Vector2f point);

	// Getter/ Setters:
	float getSpeed();
	void setSpeed(float speedIn);
	float getMaxSpeed();
	void setMaxSpeed(float speedIn);
	sf::Vector2f getDestination();
	void setDestination(sf::Vector2f point);
	void setDone(void);
	bool getDone(void);

private:
// Private Variables:
	float speed;                 // Speed car is currently travelling at.
	float maxSpeed;              // The greatest value 'speed' can take.
	float acceleration;          // The rate speed will increase between frames.
	sf::Vector2f destination;    // Location car is heading toward.
	bool done = false;           // When car finished route it becomes 'done'.
};

