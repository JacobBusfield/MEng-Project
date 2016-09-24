// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "Evolution.h"
#include <iostream>
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------------------------------//
//                                       PRIVATE PROTOTYPES  									  //
// -----------------------------------------------------------------------------------------------//
vector<vector<double>> runEvoTests(string pathDesign, string pathMap, string pathTest, int g, 
								   vector<Simulation> * sim, vector<vector<Network>> * population);
											 
void createNewPop(vector<double> * averages, int g, vector<Simulation> * sim, 
				  vector<vector<Network>> * population, Strategy strategy);
				  
vector<double> getAverages(vector<vector<double>> * generationResults, 
						   vector<vector<Network>> * population);
// -----------------------------------------------------------------------------------------------//



  
// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

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
//		2. DUPLICATE - This take the first network and duplicate it to be used for all junctions. 
//
// For evolving a single controller both methods will yield the same result.
vector<Network> evolve(string pathDesign, string pathMap, string pathTest, 
							vector<Network> n, Strategy strategy){
	// Create containers for the population of networks, and the simulations. 
	vector<vector<Network>> population;
	vector<Simulation> sim;

	// Set initial Individual controller set
	//	- SEPERATE: need inputted controllers in population (too many, too few handled by sim).
	//  - DUPLICATE: take the first network, copy it to fill the sime
	if (strategy == SEPERATE){
		population.push_back(n);
	}
	else if (strategy == DUPLICATE) {
		unsigned int duplicationNo = getRequiredOutputs(pathDesign);
		vector<Network> firstIndividual;

		for (unsigned int i = 0; i < duplicationNo; i++) {
			firstIndividual.push_back(n[0]);
		}
		population.push_back(firstIndividual);
	}

	// Set initial population of controller sets
	//	- SEPERATE: each individuals controllers need to be consistent size with random genes.
	//  - DUPLICATE: each controller is identical in an individual
	while (population.size() < POPULATION_SIZE) {
		if (strategy == SEPERATE) {
			vector<Network> individual;
			for (unsigned int i = 0; i < population[0].size(); i++) {
				individual.push_back(Network(population[0][i].getNumberOfInputs(), 
											 population[0][i].getRatioOfInputs(), 
											 population[0][i].getNumberOfOutputs(), 
											 population[0][i].getRatioOfOutputs(), 
											 population[0][i].getNumberOfGenes(), 
											 population[0][i].getNumberOfEpis()));
			}
			population.push_back(individual);
		}
		else if (strategy == DUPLICATE) {
			vector<Network> individual;
			Network newController(population[0][0].getNumberOfInputs(), 
								  population[0][0].getRatioOfInputs(), 
								  population[0][0].getNumberOfOutputs(), 
								  population[0][0].getRatioOfOutputs(), 
								  population[0][0].getNumberOfGenes(), 
								  population[0][0].getNumberOfEpis());
			for (unsigned int i = 0; i < population[0].size(); i++) {
				individual.push_back(newController);
			}
			population.push_back(individual);
		}
	}
		
	// Set Console.
	cout << fixed;
	cout << setprecision(2);
	cout << "Evolving \n";

	// Run for specified number of generations
	for (int g = 1; g <= GENERATIONS; g++) {
		cout << "Generation (" << g << "/" << GENERATIONS << ")\n";

		// Run Tests for current generation
		vector<vector<double>> generationResults = runEvoTests(pathDesign, pathMap, pathTest, g, 
															   &sim, &population);

		// Use scheme, specified in config.h, to calculate average test results
		vector<double> averages = getAverages(&generationResults, &population);

		// New 50% Pop.
		createNewPop(&averages, g, &sim, &population, strategy);
	}

	// Population's first member was the fittest from last round, so return that.
	return population[0];
}

// Generates a standardised test from a random test seed. That is the random numbers in a test file
// are set such that the test is then the same for multiple users.
void generateTest(string pathTest, string testLocation){
	// Open File
	ifstream file(pathTest);
	if (file.fail())
	{
		cout << "ERROR: can't find map file, " << pathTest << "\n";
		this_thread::sleep_for(chrono::seconds(3));
		exit(EXIT_FAILURE);
	}

	// Get Lines
	string line;
	vector<string> lines;
	while (getline(file, line)){ lines.push_back(line); }

	//	Search test file for rand. Where it is replace with actual random number.
	//	Each test can then use this same seed.
	for (unsigned int i = 0; i < lines.size(); i++) {
		if (lines[i] == "Rand") {
			lines[i + 1] = to_string(randDouble(0, stod(lines[i + 1])));
			lines.erase(lines.begin() + i);
		}
	}
	file.close();

	//	Write new file to tempory location
	ofstream eFile(testLocation);
	if (eFile.is_open()) {
		for (unsigned int i = 0; i < lines.size(); i++) {
			eFile << lines[i] << "\n";
		}
	}
	eFile.close();
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PRIVATE FUNCTIONS                                       //
// -----------------------------------------------------------------------------------------------//

// Runs mutliple tests, for a given simulation, for a desired population. Returns results where the
// inner vector is total duration of cars through the simualtion for each controller in order. The
// outer vector holds instances of the inner one for different tests.
vector<vector<double>> runEvoTests(string pathDesign, string pathMap, string pathTest, int g, 
								   vector<Simulation> * sim, vector<vector<Network>> * population){
	// container to hold results from all tests for a single generation.
	vector<vector<double>> generationResults;

	// Conduct multiple tests for each network such that the median results can be compared,
	unsigned int noOfTests = EVO_TEST_NO;
	
	// Returning the BEST network is challenging. To ensure the best is picked, many tests are 
	// conducted on the population on the last iteration (or 'saved' generation) so that 
	// statistically the 'best' has the highest probability of being returned.
	if ((g == GENERATIONS) || (g % SAVE_EVERY_N_GENS == 0)) { noOfTests *= 2; }

	for (unsigned int t = 1; t <= noOfTests; t++) {
		cout << "\tTest (" << t << "/" << noOfTests << "):";
		generateTest(pathTest, "__abc.txt");

		//	Create simulations from population
		for (unsigned int p = 0; p < population->size(); p++) {
			vector<Network*> popP;
			for (unsigned int i = 0; i < (*population)[p].size(); i++) {
				popP.push_back(&((*population)[p][i]));
			}
			sim->push_back(Simulation(pathDesign, pathMap, "__abc.txt", popP));
		}

		//	Run the simulations in seperate threads.
		vector<double> testResults;
		// omp parallel needs signed type in for loop.
#pragma omp parallel for
		for (int p = 0; p < (int)population->size(); p++) {
			(*sim)[p].run(false);
		}

		// Grab test results, and print.
		for (unsigned int p = 0; p < population->size(); p++) {
			if ((p % 5) == 0) {
				cout << "\n\t\t";
			}
			cout << (*sim)[p].getCombinedCarDuration() << "\t";
			testResults.push_back((*sim)[p].getCombinedCarDuration());
		}
		generationResults.push_back(testResults);
		cout << "\n";

		// Remove 'obvioulsy bad' individuals.
		double championTime = 0;
		for (unsigned int gr = 0; gr < generationResults.size(); gr++) {
			championTime += generationResults[gr][0];
		}
		for (unsigned int p = 1; p < population->size(); p++) {
			double competitorTime = 0;
			for (unsigned int gr = 0; gr < generationResults.size(); gr++) {
				competitorTime += generationResults[gr][p];
			}

			if (competitorTime >(championTime * MEAN_MULTIPLIER_FOR_DEATH)) {
				population->erase(population->begin() + p);
				sim->erase(sim->begin() + p);
				for (unsigned int gr = 0; gr < generationResults.size(); gr++) {
					generationResults[gr].erase(generationResults[gr].begin() + p);
				}
				p--;
			}
		}
		// Break if pop < 0.5. Note this mean pop contains the 'best' but not neccaserily in the
		// best order. This is only allowable when not on the last run (or saved run), when finding 
		// the actual champion is important.
		if (((g != GENERATIONS) && (g % SAVE_EVERY_N_GENS != 0)) 
								&& (population->size() < int(POPULATION_SIZE / 2))) {
			break; 
		}
	}
	return generationResults;
}

// Function creates new population in the GA. In recap: 
// 		1. take best 50. Fitness determined using mean of multiple tests.
// 		2. take random 3, best becomes parent A.
// 		3. take random 3, best becomes parent B.
// 		4. crossover (A,B) -> A', B' (only 70% chance of doing this step).
// 		5. mutate A', B' -> A'm, B'm.
// 		6. add A'm, B'm to population.
//
// Nuances exist for evolving multiple controllers. The user can choose one of two techniques:
// 		1. SEPERATE -  This evolves each controller passed to the function seperately. Note that 
//					   this does not use a formal cooperative evolution methodology.
//		2. DUPLICATE - This take the first network and duplicate it to be used for all junctions. 
//
// For evolving a single controller both methods will yield the same result.
void createNewPop(vector<double> * averages, int g, vector<Simulation> * sim,
				  vector<vector<Network>> * population, Strategy strategy){
	vector<vector<Network>> newPopulation;
	unsigned int champion = 0;
	vector<Simulation> newSimResults; // Prevent re-testing when selecting from new pop at start of 
									  // loop again.

	// Print average test results.
	cout << "\tCALC (" << 1 << "/" << 1 << "):";
	for (unsigned int i = 0; i < averages->size(); i++) {
		if ((i % 5) == 0) {
			cout << "\n\t\t";
		}
		cout << (*averages)[i] << "\t";
	}
	cout << "\n";

	// Grab next best from population
	for (unsigned int p = 0; p < (unsigned int)(POPULATION_SIZE / 2); p++) {
		if (p < population->size()) {
			champion = 0;
			for (unsigned int i = 1; i < averages->size(); i++) {
				if ((*averages)[i] <= (*averages)[champion]) {
					champion = i;
				}
			}
			newPopulation.push_back((*population)[champion]);
			newSimResults.push_back((*sim)[champion]);
			population->erase(population->begin() + champion);
			averages->erase(averages->begin() + champion);
			sim->erase(sim->begin() + champion);
		}
		else { break; }
	}
	population->erase(population->begin(), population->end());
	sim->erase(sim->begin(), sim->end());
	*population = newPopulation;
	*sim = newSimResults;

	// If save sequence (or last), print Champion Network.
	//	- SEPERATE: need to print out every controller
	//  - DUPLICATE: just print it out once.
	if ((g%SAVE_EVERY_N_GENS == 0) || (g == GENERATIONS)) {
		if (strategy == SEPERATE) {
			for (unsigned int i = 0; i < (*population)[0].size(); i++) {
				stringstream  networkPrintFile;
				networkPrintFile << "EVOLVED_GEN_" << g << "_Controller_" << i << ".txt";
				string npf = networkPrintFile.str();
				(*population)[0][i].printNetwork(npf);
			}
		}
		else if (strategy == DUPLICATE) {
			stringstream  networkPrintFile;
			networkPrintFile << "EVOLVED_GEN_" << g << ".txt";
			string npf = networkPrintFile.str();
			(*population)[0][0].printNetwork(npf);
		}
	}

	//	Create population (controllers) via mutation
	while (population->size() < POPULATION_SIZE) {
		vector<Network> A, B, AA, BB;

		// Grab random 3 (twice)
		for (int i = 0; i < 2; i++) {
			vector<vector<Network>> temp;
			vector<int> tempIndex;
			for (int j = 0; j < 3; j++) {
				// Use newPopulation size as it will remain the 'half' through multiple cycles.
				tempIndex.push_back(randInt(0, newPopulation.size() - 1)); 
				temp.push_back((*population)[tempIndex.back()]);
			}
			int tempChampion = 0;
			for (unsigned int j = 1; j < temp.size(); j++) {
				if ((*sim)[tempIndex[j]].getCombinedCarDuration() 
				 <= (*sim)[tempIndex[tempChampion]].getCombinedCarDuration()) {
					tempChampion = j;
				}
			}
			if (i == 0) { A = (*population)[tempIndex[tempChampion]]; }
			else if (i == 1) { B = (*population)[tempIndex[tempChampion]]; }
		}

		// 70% of time crossover
		if (randDouble(0, 1) <= 0.7) {
			vector<Network> ATemp = A;
			if (strategy == SEPERATE) {
				for (unsigned int i = 0; i < A.size(); i++) {
					AA.push_back(A[i].crossover(B[i], 3));
					BB.push_back(B[i].crossover(ATemp[i], 3));
				}
			}
			else if (strategy == DUPLICATE) {
				Network A0Copy = A[0];
				Network ATemp = A[0].crossover(B[0], 3);
				Network BTemp = B[0].crossover(A0Copy, 3);
				for (unsigned int i = 0; i < A.size(); i++) {
						AA.push_back(ATemp);
						BB.push_back(BTemp);
				}
			}
		}
		else { AA = A; BB = B; }

		// Mutate A, mutate B
		vector<Network> AAm, BBm;
		if (strategy == SEPERATE) {
			for (unsigned int i = 0; i < A.size(); i++) {
				AAm.push_back(AA[i].mutate(MUTATION_RATE));
				BBm.push_back(BB[i].mutate(MUTATION_RATE));
			}
		}
		else if (strategy == DUPLICATE) {
			Network ATemp = AA[0].mutate(MUTATION_RATE);
			Network BTemp = BB[0].mutate(MUTATION_RATE);
			for (unsigned int i = 0; i < A.size(); i++) {
				AAm.push_back(ATemp);
				BBm.push_back(BTemp);
			}
		}

		// Add A
		population->push_back(AAm);

		// If space add B.
		if (population->size() < POPULATION_SIZE) { population->push_back(BBm); }
	}
	// Clear sim - dont need these results again
	sim->erase(sim->begin(), sim->end());
}

// Gets the average result from the mutliple tests conducted for each individual in a population. 
// The vector returned is the average for each controller in order.
vector<double> getAverages(vector<vector<double>> * generationResults, 
						   vector<vector<Network>> * population){
	vector<double> averages;

	//	For each member of the population ...
	for (unsigned int p = 0; p < population->size(); p++) {
		// ... Get the results from each test.
		vector<double> testResults;
		for (unsigned int i = 0; i < generationResults->size(); i++) {
			testResults.push_back((*generationResults)[i][p]);
		}
#if TEST_SELECTION == MEDIAN
		averages.push_back(getMedian(testResults));
#elif TEST_SELECTION == MEAN
		averages.push_back(getMean(testResults));
#endif
	}
	
	// Print mean, average
	fstream file;
	file.open(".\\Matlab\\4WJ_data_durations_populations.txt",
			  fstream::in | fstream::out | fstream::app);
	file << getMean(averages) << "\n";

	return averages;
}

// -----------------------------------------------------------------------------------------------//