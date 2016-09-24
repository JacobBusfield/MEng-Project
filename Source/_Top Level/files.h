// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include "stdafx.h"


// ---------------------------------------------------------------------------------------------- //
//										SIMULATION FILES				 				          //
// ---------------------------------------------------------------------------------------------- //
// 4 way junction X 4 (traffic network)
#define SIM_4WJX4			".\\Loadable\\Sim Designs\\4WJx4\\4WJx4_sim.txt"
#define MAP_4WJX4			".\\Loadable\\Sim Designs\\4WJx4\\4WJx4_map.txt"
#define MAP_4WJX4_HIERARCHY	".\\Loadable\\Sim Designs\\4WJx4\\4WJx4_map_hierarchy.txt"
#define TEST_4WJX4			".\\Loadable\\Sim Designs\\4WJx4\\4WJx4_test.txt"

// 4 way junction
#define SIM_4WJ				".\\Loadable\\Sim Designs\\4WJ\\4WJ_sim.txt"
#define MAP_4WJ				".\\Loadable\\Sim Designs\\4WJ\\4WJ_map.txt"
#define TEST_4WJ			".\\Loadable\\Sim Designs\\4WJ\\4WJ_test.txt"
#define TEST_4WJ_UNBALANCED	".\\Loadable\\Sim Designs\\4WJ\\4WJ_test_unbalanced.txt"
#define TEST_4WJ_LONG		".\\Loadable\\Sim Designs\\4WJ\\4WJ_test_longer.txt"

// 3 way junction
#define SIM_3WJ				".\\Loadable\\Sim Designs\\3WJ\\3WJ_sim.txt"
#define MAP_3WJ				".\\Loadable\\Sim Designs\\3WJ\\3WJ_map.txt"
#define TEST_3WJ			".\\Loadable\\Sim Designs\\3WJ\\3WJ_test.txt"

// Original network to test simulation
#define SIM_ORIG			".\\Loadable\\Sim Designs\\Test\\_sim.txt"
#define MAP_ORIG			".\\Loadable\\Sim Designs\\Test\\_map.txt"
#define TEST_ORIG			".\\Loadable\\Sim Designs\\Test\\_sim_train1.txt"
// ---------------------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------------------- //
//						                 CONTROLLER FILES			                 		      //
// ---------------------------------------------------------------------------------------------- //
// Single 3 way-junction controller
#define CONT_3WJ			".\\Loadable\\Controllers\\3WJ_SINGLE.txt"

// Single 4 way-junction controller
#define CONT_4WJ			".\\Loadable\\Controllers\\4WJ_SINGLE.txt"

// Duplicate controller for 4, 4-way junction.
#define CONT_4WJX4			".\\Loadable\\Controllers\\4WJX4_DUPLICATE.txt"


// --- FOLLOWING CONTROLLERS ARE POOR. --- //

// Single giant (hierarchical) controller for 4, 4-way junction.
#define CONT_4WJX4_H		".\\Loadable\\Controllers\\4WJX4_HIERARCHY.txt"

// Seperate controllers for 4, 4-way junction.
#define CONT_4WJX4_S1		".\\Loadable\\Controllers\\4WJX4_SEPERATE_S1.txt"
#define CONT_4WJX4_S2		".\\Loadable\\Controllers\\4WJX4_SEPERATE_S2.txt"
#define CONT_4WJX4_S3 		".\\Loadable\\Controllers\\4WJX4_SEPERATE_S3.txt"
#define CONT_4WJX4_S4		".\\Loadable\\Controllers\\4WJX4_SEPERATE_S4.txt"
// ---------------------------------------------------------------------------------------------- //