// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "car.h"
#include "road.h"
#include "system.h"
#include "location.h"
#include "route.h"
#include "config.h"
#include "SFML/Graphics.hpp"
#include "Network.h"
#include "MOE.h"
#include <thread>         
#include <chrono>        
#include <iostream>
#include <sstream>


// Class: 
//
// The top level Simulation object. Each instance creates a simulation using the files passed to it.
// This is either using a round-robin (if epiNet controller is not specified in constructor) or with
// epiNet controller if specified. The files to be included have the following jobs:
//
//   - Design: declares new locations and systems, and what their two dimensional coordinates are.
//             Specifies which are connected together via roads.
//
//	 - Test:   declares if, and when, a location will spawn a car. Determines whether these cars 
//             will have a route, and what that route is. Test variables can be randomised over a  
//             desired scale.
//
//   - Map:    used to map a system's controller inputs and outputs to epiNet.
//
// The simulation can be run as many times as desired by using the run() function. The run() 
// function does not return until the simulation is complete/ exited. Performace metrics for the
// simulation can be taken after the simulation is run() as this class...
//
// Inherits:
//   MOE - Used to store and retrieve measures of effectiveness for the simulation (i.e durations).
class Simulation : public MOE
{
public:
// Constructor:
	// Used to specify simulation with at least one epiNet controller.
	Simulation(std::string pathDesign, std::string pathMap, std::string pathTest, 
			   std::vector<Network*> n);
	// Used to specify simulation with no epiNet controller (all therefore use round-robin).
	Simulation(std::string pathDesign, std::string pathTest);
	// Default destructor.
	~Simulation();

	
// Public Functions:
	// Simulation can be run with or without GUI. If GUI is specified a new (SFML) render window is
	// opened and a simulation graphically displayed within it. This simulation's speed can be 
	// controlled using the arrow keys, and exited. If no GUI the simulation runs as fast as
	// possible. This function only returns after the simulation has completed.
	void run(bool showGUI = true);
	

private:
// Private Type Declarations:	
	typedef struct {							// Parsed from test file.
		std::vector<std::vector<Car>> c;          // Sets of cars (for each location) to be spawned.
		std::vector<unsigned int> lNo;            // Location corresponding set is to be spawned.
		std::vector<std::vector<unsigned int>> d; // Index in each set that a delay is required.
	} lCars;

	enum Speed {FAST, EXACT, STOPPED};		    // Speed settings (for simulation's run speed).
	
	
// Private Functions:	
	// EpiNet controllers require updating to process inputs into new outputs. Inputs and outputs 
	// have already been mapped by constructor to inN and outN so by calling this function each 
	// controller will automatically recieve updated commands.
	void updateEpi(void);

	// Run function calls similar methods depending whether window is required or not. These
	// functions hold the main 'game loop'. That is update, draw, display repeatedly running until
	// simulation completes.
	void runWindow(std::vector<Location> * l, std::vector<System> * s, std::vector<Road> * r,
				   std::vector<Car> * car);
	void runNoWindow(std::vector<Location> * l, std::vector<System> * s, std::vector<Road> * r,
	                 std::vector<Car> * car);

	// Functions used to parse config files and update objects passes to them. Also exit program if
	// file cannot be found.
	void parseSim(std::vector<Location> * l, std::vector<System> * s, std::vector<Road> * r);
	void parseMap(std::vector<System> * s);
	void parseTest(std::vector<Location> * l, std::vector<System> * s, std::vector<Car> * c);
	
// Private Variables:	
	std::string designPath;		// Used to store path to Design file.
	std::string mapPath = "na";	// Used to store path to Map file ("na" means no epiNet by default).
	std::string testPath;       // Used to store path to Design file.
	Speed simSpeed = EXACT;		// Simulation's current run speed.
	sf::Texture texture_car;    // Car texture. Loaded once at top level to reduce CPU Time.
	std::vector<Network*> n; 	// Networks used by simulation
	std::vector<std::vector<double>> inN; // (All) inputs that can be passed to the network.
	std::vector<std::vector<double>> outN;// (All) outputs that can be got from the network.
	VarClock timeout;		// Clock used to terminate simulation after predefined time has
							// elapsed. Length defined in config.h
	int epiCounter = 0;		// EpiNet is not updated on each frame, this tracks progress through
							// multiple defined in config.h.
	bool firstRun = true;	// Some processes require conducting when the program is first run().
	sf::Vector2i rwPos;		// Tracks position of render window. If user moves it, closes it, and
							// runs it again, window will open at position moved to.
	sf::Vector2u rwSize;	// Tracks size of render window. If user re-sizes it, closes it, and
							// runs it again, window will by the size it was changed to.
};

// General function that can be used to determine how many systems there are in a design files.
unsigned int getRequiredOutputs(std::string fileName);