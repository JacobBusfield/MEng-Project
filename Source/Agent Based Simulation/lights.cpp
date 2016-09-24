// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "lights.h"

// -----------------------------------------------------------------------------------------------//
//                                          CONSTRUCTORS                                          //
// -----------------------------------------------------------------------------------------------//

// Default: required by Road class which inherits the class.
Lights::Lights(){}
Lights::~Lights(){}

// Creates lights at given x, y position, in line with the vector (starting with the red light).
Lights::Lights(const float x, const float y, sf::Vector2f unitDir)
{
	// Traffic lights
	initLight(&r, sf::Color::Red);
	initLight(&a, sf::Color::Yellow);
	initLight(&g, sf::Color::Green);
	r.setPosition(x,y);
	a.setPosition(x + LIGHT_RADIUS * 2 * unitDir.x, y + LIGHT_RADIUS * 2 * unitDir.y);
	g.setPosition(x + LIGHT_RADIUS * 4 * unitDir.x, y + LIGHT_RADIUS * 4 * unitDir.y);


	// Progress Bar
	double angle = (((atan2(unitDir.y, unitDir.x)) * 180) / 3.14159265) -90;
	bar.setFillColor(sf::Color::Red);
	bar.setOrigin((ROAD_THICKNESS / 2), LIGHT_RADIUS+1); // +1 gives nice overlap with junction
	bar.setSize(sf::Vector2f(ROAD_THICKNESS, 0));
	bar.setRotation((float)angle);
	bar.setPosition(x, y);
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Updates lights. Changes colours to state s, change waiting bar depending on the time. Time is
// expected to be in the range between 0 - 1.
void Lights::update(State s, double time)
{
	// Change lights depending on state, transparent == off.
	switch (s) {
		case GO:
			r.setFillColor(sf::Color::Transparent);
			a.setFillColor(sf::Color::Transparent);
			g.setFillColor(sf::Color::Green);
			break;

		case PREPARE_TO_GO:
			r.setFillColor(sf::Color::Red);
			a.setFillColor(sf::Color::Yellow);
			g.setFillColor(sf::Color::Transparent);
			break;

		case PREPARE_TO_STOP:
			r.setFillColor(sf::Color::Transparent);
			a.setFillColor(sf::Color::Yellow);
			g.setFillColor(sf::Color::Transparent);
			break;

		case STOP:
			r.setFillColor(sf::Color::Red);
			a.setFillColor(sf::Color::Transparent);
			g.setFillColor(sf::Color::Transparent);
			break;
	}
	
	// Change bar waiting time. Protect against weird values.
	// Bar is length of lights when time == 1, hence 6 * lights radius at 1.
	if (time > 1){ time = 1; }
	else if (time < 0 ){ time = 0; }
	bar.setSize(sf::Vector2f((float)ROAD_THICKNESS, (float)(time * LIGHT_RADIUS * 6)));
}

// Function to draw lights to the SFML render window.
void Lights::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	// progress bar behind lights.
	target.draw(bar);
	target.draw(r);
	target.draw(a);
	target.draw(g);
}

// -----------------------------------------------------------------------------------------------//



// -----------------------------------------------------------------------------------------------//
//                                       PRIVATE FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Function for creating an individual light.
void Lights::initLight(sf::CircleShape * l, sf::Color c){
	l->setRadius(LIGHT_RADIUS);
	l->setOrigin(LIGHT_RADIUS, LIGHT_RADIUS);
	l->setFillColor(c);
	l->setOutlineColor(sf::Color::White);
	l->setOutlineThickness(2);
}

// -----------------------------------------------------------------------------------------------//