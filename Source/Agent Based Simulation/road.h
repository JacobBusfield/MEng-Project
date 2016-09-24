// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "SFML/Graphics.hpp"
#include "car.h"
#include "node.h"
#include "lights.h"
#include "varClock.h"

// Class: Road
//
// This class is responsible for simulating a road within the agent based simulation as it transports
// cars between two nodes. The road has 2 lanes to allow traffic to be transported in either direction.
// Each lane is attached to the road end from which traffic comes from to drive along it. The road 
// remembers the two nodes it is attached to, and will access function from them to determine what the
// state of its end traffic lights are, and to pass/ get cars from them.
class Road : public sf::Drawable
{
public:
// Constructor:
	// Constructs road object, and sets speed limit of road.
	Road(const float speedLimit);
	
// Public Functions:
	// Connects two node, n1 n2, together using a straight road between both point.
	void connect(Node * n1, Node * n2);

	// Draw the road onto the SFML render window.
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	// Updates cars within the road (handles grabbing, and passing, them to connected nodes).
	void update(void);
	
	// Add car to road, coordinates passed to this function determine which side is connected to the
	// object wanting to add cars.
	void add(Car * newCar, sf::Vector2f side);
	
	// Returns position of the side road opposite to the one given by the coords passed in.
	sf::Vector2f getOtherSidePosition(sf::Vector2f pos);
	
	// Returns Node pointer for the road end opposite to the one given by the coords passed in.
	Node * getOtherSideConnection(sf::Vector2f pos);
	
	// Returns tail car leaving from a position, returns null if none exist.
	Car* getCarLeaving(sf::Vector2f pos);
	
	// Return head car entering a position, return null if none exist. 
	Car* getCarEntering(sf::Vector2f pos);
	
	// Return true if car is waiting at end of the road (waiting on red light).
	bool carWaiting(sf::Vector2f pos);
	
	// Return true if the end of the road is stopped (red lights).
	bool stopped(sf::Vector2f pos);

private:
// Private Type Definition
	typedef struct {
		sf::Vector2f pos;					// Position of road end.
		std::vector<Car*> car;				// Cars moving to other side. i.e a.cars move towards b.
		Node * connection, *altConnection;	// Each road end has access to its node and other side.
		sf::Vector2f path;					// Vector between current node and opposite one.
		sf::Vector2f unitPath;				// Normalised 'path', computing once saves CPU time.
		State state = STOP;					// Traffic light state for cars leaving at this end.
		Lights lights;						// Drawable traffic lights to be placed on road end.
	}RoadEnd;

// Private Functions:
	// Updates Cars on single side of a road (cars heading away from roadEnd specified).
	void updateQueue(RoadEnd * r);
	
	// Adds where the car should start (Road starts in Node)
	void addCarStart(Car * newCar, RoadEnd * r);
	
	// Adds where the car should stop (before the connecting node) 
	void addCarEnd(Car * newCar, RoadEnd * r);
	
	// Updates state of lights for road end. This includes transition time/ states between STOP and 
	// GO states.
	void updateState(RoadEnd * r);
	
	// For specified road end, update the lights depending on state of the road end.
	void updateLights(RoadEnd * r);

	
// Private Variables:
	RoadEnd a, b; 	         // Structure with useful information for each end of the road.
	float speed; 			 // Speed of road (same both sides)
	VarClock clock;          // Clock that determines the state of lights in the system
	sf::Vertex vertices[4];  // Variable to draw road
	float thickness;		 //   "               "
	sf::Color color;		 //   "               "
};