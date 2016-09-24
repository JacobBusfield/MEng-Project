// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "simulation.h"
#include "network.h"
#include "Evolution.h"
#include "files.h"
#include "functions.h"
#include "SFML/Graphics.hpp"



int main(int argc, char** argv)
{
	// --- DEMO --- //
	// The following code is presented to demo the main features.
	// Code can be uncommented to trial.
	// Use config.h to change evolution settings as desired.


	// Evolving a controller from scratch, running comparisons tests with round robin,
	// and running graphically.
		// Network n0(4, 0.5, 2, 0.4, 50, 3);
		// std::vector<Network> evolved = evolve(SIM_4WJ, MAP_4WJ, TEST_4WJ_LONG, { n0 });
		// compareTest({ &evolved }, SIM_4WJ, MAP_4WJ, TEST_4WJ);
		// run({ &evolved }, SIM_4WJ, MAP_4WJ, TEST_4WJ, BOTH);


	// Run 4WJ controller.
		// Network evolved4WJ = Network::readInNetwork(CONT_4WJ);
		// run({ &evolved4WJ }, SIM_4WJ, MAP_4WJ, TEST_4WJ, BOTH);

	// Run duplicate controllers
		 Network evolved0 = Network::readInNetwork(CONT_4WJX4);
		 Network evolved1 = Network::readInNetwork(CONT_4WJX4);
		 Network evolved2 = Network::readInNetwork(CONT_4WJX4);
		 Network evolved3 = Network::readInNetwork(CONT_4WJX4);
		 run({ &evolved0, &evolved1, &evolved2, &evolved3 }, SIM_4WJX4, MAP_4WJX4, TEST_4WJX4, BOTH);

	// Evolve separate controllers
		// Network n1(4, 0.5, 2, 0.4, 50, 3);
		// Network n2(4, 0.5, 2, 0.4, 50, 3);
		// Network n3(4, 0.5, 2, 0.4, 50, 3);
		// Network n4(4, 0.5, 2, 0.4, 50, 3);
		// std::vector<Network> evolvedSep = evolve( SIM_4WJX4, MAP_4WJX4, TEST_4WJX4, { n1,n2,n3,n4 }, SEPERATE);
		// run({ &evolvedSep[0], &evolvedSep[1], &evolvedSep[2], &evolvedSep[3] }, SIM_4WJX4, MAP_4WJX4, TEST_4WJX4, BOTH);


	// Evolve duplicate controllers
		 //Network nd(4, 0.5, 2, 0.4, 50, 3);
		 //std::vector<Network> evolvedd = evolve( SIM_4WJX4, MAP_4WJX4, TEST_4WJX4, { nd,nd,nd,nd }, DUPLICATE);
		 //run({ &evolvedd[0], &evolvedd[1], &evolvedd[2], &evolvedd[3] }, SIM_4WJX4, MAP_4WJX4, TEST_4WJX4, BOTH);
}

