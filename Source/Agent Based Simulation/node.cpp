// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "node.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

Node::Node() {}
Node::~Node() {}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Optional initialisation for circleShape this class inherits. Useful for the location and system
// classes which inherit this class to remain consistent (style-wise).
void Node::init(const sf::Vector2f p){
	setPosition(p);
	setRadius(RADIUS);
	setOrigin(RADIUS, RADIUS);
	setFillColor(sf::Color(90, 90, 90));
	setOutlineColor(sf::Color::Red);
	setOutlineThickness(3);
}

// -----------------------------------------------------------------------------------------------//