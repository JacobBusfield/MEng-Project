// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "SFML/Graphics.hpp"
#include "config.h"

// MACROS
#define LIGHT_RADIUS 10 // Radius of the each traffic light circle (3 for red, amber and green).

// ENUMERATED TYPES
enum State {		// Road States.
	GO,				// Green lights, car can move into node junction.
	STOP,			// Red Lights, cars must wait on road.	
	PREPARE_TO_GO,  // Amber light, cars must wait on road.
	PREPARE_TO_STOP // Red and amber lights, cars must wait on road.	
};


// Class: Lights
//
// Three vertically alligned circles that can be used to show a light sequence with 4 states. And a
// waiting bar that stretches length of the lights. The class is drawable (SFML) as it...
//
// Inherits:
//   Drawable - SFML base class that contains functions to allow class to be drawn to SFML render
//              window.
class Lights : public sf::Drawable
{
public:
// Constructor:
	// Default: required by Road class which inherits the class.
	Lights();
	~Lights();

	// Creates lights at given x, y position, in line with the vector (starting with the red light).
	Lights(const float x, const float y, sf::Vector2f unitDir);

// Public Functions:
	// Updates lights. Changes colours to state s, change waiting bar depending on the time (0-1).
	void update(State s, double time);
	
	// Function to draw lights to the SFML render window.
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
// Private Functions:
	// Function for creating an individual light.
	void initLight(sf::CircleShape * l, sf::Color c);

// Private Variables:
	sf::CircleShape r; 		// Red light
	sf::CircleShape a;		// Amber light
	sf::CircleShape g;		// Green light
	sf::RectangleShape bar;	// Waiting Bar
};

