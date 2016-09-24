// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"
#include "Network.h"
#include "MOE.h"
#include "simulation.h"
#include "config.h"
#include <vector>
#include <fstream>


enum Strategy { // Used to specify GA methodology for mutliple controllers.
	SEPERATE,   // Seperate controllers with distinct genotypes.
	DUPLICATE   // Duplicate controllers with identical genotypes.
};

// Function used to evolve epiNet networks (n) for given sim design, test and mapping. The bases of 
// the function is a GA with the following methodology:
// 		1. take best 50%. Fitness determined using mean of multiple tests.
// 		2. take random 3, best becomes parent A.
// 		3. take random 3, best becomes parent B.
// 		4. crossover (A,B) -> A', B' (only 70% chance of doing this step).
// 		5. mutate A', B' -> A'm, B'm.
// 		6. add A'm, B'm to population.
//
// Nuances exist for evolving multiple controllers. The user can choose one of two techniques:
// 		1. SEPERATE -  This evolves each controller passed to the function seperately. Note that 
//					   this does not use a formal cooperative evolution methodology.
//		2. DUPLICATE - This take the first network and duplicated it to be used for all junctions. 
//
// For evolving a single controller both methods will yield the same result.
std::vector<Network> evolve(std::string pathDesign, std::string pathMap, std::string pathTest,
							std::vector<Network> n, Strategy strategy = SEPERATE);
							
// Generates a standardised test from a random test seed. That is the random numbers in a test file
// are set such that the test is then the same for multiple users.
void generateTest(std::string pathTest, std::string testLocation);

