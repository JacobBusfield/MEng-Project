// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "car.h"
#include "config.h"
#include "varClock.h"
#include "SFML/Graphics.hpp"

// Forward declarations.
class Road;

// Class: Node
//
// This virtual class is used so that locations and systems (Location.h and System.h) which inherit
// it can be treat identically. It also defines the standard look (circular road) of a node by...
//
// Inherits:
//   CircleShape - This SFML Class represents a circle that can be drawn on the SFML render window.
class Node : public sf::CircleShape
{
public:
// Constructor:
	// default.
	Node();
	~Node();

// Virtual Functions.
	virtual void update(void) = 0;
	virtual bool add(Car * newCar) = 0;
	virtual Car* get(Road * n) = 0;
	virtual void connect(Road* r) = 0;
	virtual bool checkConnection(Road * r) = 0;
	virtual double getWaitDuration(Road * r) = 0;

// Public Functions.
	// Optional initialisation for circleShape this class inherits. Useful for the location and system
	// classes which inherit this class to remain consistent (style-wise).
	void init(const sf::Vector2f p);
};


