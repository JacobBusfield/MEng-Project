// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once


// ---------------------------------------------------------------------------------------------- //
//										   RENDER WINDOW     			 				          //
// ---------------------------------------------------------------------------------------------- //
// (unsigned int) Height and width of the render window.
#define WIDTH 1600
#define HEIGHT 900
// ---------------------------------------------------------------------------------------------- //




// ---------------------------------------------------------------------------------------------- //
//									  SIMULATION ATTRIBUTES    			 				          //
// ---------------------------------------------------------------------------------------------- //
// (bool) determines whether graphical display is shown during simulation or not.
#define GUI true		

// (float) determines radius of nodes (connections ending roads).				
#define RADIUS 40.f			

// (float) determines width of the road (connections ending roads). 			
#define ROAD_THICKNESS 40.f			

// (float) rate at which cars turn in systems (~0.1 is expected value).	
#define TURN_RATE (FRAME_TIME * 200)	

// (float) time, in seconds, that cars will spawn from locations.
#define SPAWN_RATE (FRAME_TIME * 200)

// (int) no. of cars initialised by the simulation; using more than this number will cause a fault.
#define MAX_CARS 300

// (float) amount of anti-alias to use on simulation, maximum value depends on graphics card, and
// will be auto-limited. 					
#define ANTIALIAS_LEVEL 8		

// (float) rate at which the speed of a car will increase per draw cycle.		
#define ACCLERATION (FRAME_TIME/4)		

// (int) number of pixels which will always seperate any two cars, 0 will mean the cars just touch.
#define MIN_SEPERATION 3	

// (int) minimum time, in seconds, between light transition: GO - > PREPARE TO STOP.			
#define TIME_PTS 2		

// (int) minimum time, in seconds, between light transition: STOP - > PREPARE TO GO.				
#define TIME_PTG 2	

// (int) minimum time, in seconds, of GO state.					
#define TIME_GO 5	

// (int) maximum time, in seconds, of GO state (on top of TIME_GO).					
#define TIME_GO_MAX 10			

// (int) multiple of the frame time, that epinet should execute. 100x0.01 = every second. 		
#define EPI_UPDATE_TICKS 100 

// (float) how long it takes for car waiting to fill waiting bar (set controller input to 1).
#define TIMEOUT_WAITING_CAR_INPUT 10   

// (float) "time" expended in each frame update. i.e 0.01 = 100 fps. Changing frame_time will change
// simulation behaviour. A low value may not be able to be handled by the hardware, this will result
// in the simulation being slower than the exact time. */
#define FRAME_TIME 0.01       

// (float) How long before simulation times out (in second where each frame takes FRAME_TIME).
#define SIM_TIMEOUT 500
// ---------------------------------------------------------------------------------------------- //




// ---------------------------------------------------------------------------------------------- //
//										    EVOLUTION       			 				          //
// ---------------------------------------------------------------------------------------------- //
// (unsigned int) Number of generations GA should run for.
#define GENERATIONS 100

// (unsigned int) Generation interval between saving controller to text file during evolution.
#define SAVE_EVERY_N_GENS 50

// (unsigned int) Number of tests to be run per generation (doubled when controller is saved).
#define EVO_TEST_NO 5

// (unsigned int) Number of individuals in population for GA.
#define POPULATION_SIZE 5

// (float) percentage of epiNet's genome and epigenome that should be mutated by GA. 
#define MUTATION_RATE 0.05
	
// (MEDIAN OR MEAN) How multiple fitness tests should be averaged within the GA.
#define MEDIAN 0 // <- Don't edit //
#define MEAN 1   // <- Don't edit //
#define TEST_SELECTION MEDIAN

// (unsigned int) Individuals with average fitness values that when divided by this value are still
// greater than the champion fitness are immediately killed (removed from GA).
#define MEAN_MULTIPLIER_FOR_DEATH 2
// ---------------------------------------------------------------------------------------------- //




// ---------------------------------------------------------------------------------------------- //
//										        MISC.       			 				          //
// ---------------------------------------------------------------------------------------------- //
// (unsigned int) Number tests to be run when using compareTest() in functions.cpp
#define COMPARISON_TESTS 10
// ---------------------------------------------------------------------------------------------- //









