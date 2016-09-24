// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "simulation.h"
#include "network.h"
#include "Evolution.h"
#include "files.h"
#include "SFML/Graphics.hpp"

// Public defintion. Used to specify what controller set up should be displayed.
enum Displayed{
	RR,		// Round-Robin
	EPI,	// EpiNet
	BOTH    // Both Round-Robin and EpiNet
	};

	
// Used to compare a traffic network controlled by epiNet against the round-robin equivilant.
// Results are stored to a text file which by default take the prefix: UNNAMED.
void compareTest(std::vector<Network*> n, std::string sim, std::string map, std::string test, 
				 std::string name = "UNNAMED");


// Runs either the Round-robin, epiNet, or both versions of a traffic simulation. Both versions are
// run in seperate threads.	 
void run(std::vector<Network*> n, std::string sim, std::string map, std::string test, Displayed d);

