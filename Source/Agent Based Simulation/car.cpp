// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "car.h"      // Forward declaration.
#include "location.h" // Forward declaration.
#include "SFML/Graphics.hpp"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Creates an instance of the car with desired texture.
// Initial values are picked arbitarily.
Car::Car(sf::Texture * texture) {
	setTexture(*texture);
	setOrigin(getLocalBounds().width / 2, getLocalBounds().height);
	setScale(0.2f, 0.2f);
	acceleration = (float)ACCLERATION;
	speed = 0;
	maxSpeed = 0.05f;
	setPosition(sf::Vector2f(-100, -100)); // off screen.
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Drives car to coordinates given by destination. Car stops, and return true when complete.
bool Car::drive(sf::Vector2f destination) {
	// Use destination to work out vector (path) that needs to be moved. From this calculate
	// if the car, at its current speed can move in this distance, else brake.
	sf::Vector2f path = destination - getPosition();
	sf::Vector2f unitPath = path / std::sqrt(path.x*path.x + path.y*path.y);

	// If car is stationary, give kick forward ready for coming multiplication accelaration.
	if (speed <= 0) { speed = 0.01f; }

	// If room to drive car at current speed before destination -> do so.
	if (speed <= std::sqrt(path.x*path.x + path.y*path.y)) {
		// Need if statement to handle undefined behaviour of multiplying unitPath.x by 0. 
		// (0.000000000000001 is fine however).
		if (speed > 0) { move(unitPath.x * speed, unitPath.y * speed); }

		// moved by speed, assume can speed up by max acceleration. If cannot, next drive() will 
		// handle it.
		if (speed < maxSpeed) { speed += acceleration; }
		else if (speed > maxSpeed){ speed = maxSpeed; }
	}
	// Move to destination & return true to confirm destination has been reached.
	else {
		this->setPosition(destination);
		return true;
	}
	return false;
}

 
// Determines if there is room between the current car's position and the 'point' + some 'space' for
// a car to be created.
bool Car::canSpawn(float distance, sf::Vector2f point) {
	// Spawn room : radius of node + width of new car + half with of current car.
	float spawnRoom = distance + (this->getScale().x * this->getLocalBounds().width * 1.5f);
	sf::Vector2f spaceVector = point - this->getPosition();
	float space = std::sqrt(spaceVector.x*spaceVector.x + spaceVector.y*spaceVector.y);

	// True if there is room to spawn a car.
	if (space > spawnRoom) { return 1; }
	return 0;
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                  GETTER AND SETTER METHODS.                                    //
// -----------------------------------------------------------------------------------------------//
float Car::getSpeed() {
	return speed;
}

void Car::setSpeed(float speedIn) {
	speed = speedIn;
}

float Car::getMaxSpeed()
{
	return maxSpeed;
}

void Car::setMaxSpeed(float speedIn){
	maxSpeed = speedIn;
}

sf::Vector2f Car::getDestination()
{
	return destination;
}

// Angle calulated from standard 'east' mathematical position and converted to degrees.
// Car is then rotated to face the destination.
void Car::setDestination(sf::Vector2f point){
	destination = point;
	double angle = ((atan2(destination.y - getPosition().y, destination.x - getPosition().x)) * 180)
				   / 3.14159265;
	setRotation((float)angle);
}

void Car::setDone(void){
	done = true;
	routeDone();
}

bool Car::getDone(void)
{
	return done;
}
// -----------------------------------------------------------------------------------------------//
