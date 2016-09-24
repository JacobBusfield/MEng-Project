// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "functions.h"

// Used to compare a traffic network controlled by epiNet against the round-robin equivilant.
// Results are stored to a text file which by default take the prefix: UNNAMED.
void compareTest(std::vector<Network*> n, std::string sim, std::string map, std::string test, std::string name) {
	// Create and open files to store results in
	std::stringstream  file1Location;
	file1Location << ".\\Matlab\\" << name << "_durations_unimproved" << ".txt";
	std::string f1l = file1Location.str();

	std::stringstream  file2Location;
	file2Location << ".\\Matlab\\" << name << "_durations_evolved" << ".txt";
	std::string f2l = file2Location.str();

	std::fstream file1;
	file1.open(f1l, std::fstream::in | std::fstream::out | std::fstream::app);
	std::fstream file2;
	file2.open(f2l, std::fstream::in | std::fstream::out | std::fstream::app);

	// Conduct specified number of tests.
	for (int i = 1; i < COMPARISON_TESTS; i++) {
		// Each iteration generate new test from random template.
		std::cout << i << "/" << COMPARISON_TESTS << "\n";
		generateTest(test, "__abc.txt");

		// Run first simulation
		Simulation s1(sim, "__abc.txt");
		s1.run(false);
		std::vector<double> durations1 = s1.getCarDurations();
		for (unsigned int d = 0; d < durations1.size(); d++) { file1 << durations1[d] << " "; }
		file1 << "\n";

		// Run second simulation
		Simulation s2(sim, map, "__abc.txt", n);
		s2.run(false);
		std::vector<double> durations2 = s2.getCarDurations();
		for (unsigned int d = 0; d < durations2.size(); d++) { file2 << durations2[d] << " "; }
		file2 << "\n";
	}
}

// Runs either the Round-robin, epiNet, or both versions of a traffic simulation. Both versions are
// run in seperate threads.	
void run(std::vector<Network*> n, std::string sim, std::string map, std::string test, Displayed d) {
	Simulation s1(sim, "__abc.txt");
	Simulation s2(sim, map, "__abc.txt", n);
	
	while (1) {
		// Generate unique test that can be used by both simulations.
		generateTest(test, "__abc.txt");

		if (d == RR) { s1.run(); }
		else if (d == EPI) { s2.run(); }
		else if (d == BOTH) {
			// Run both simulations in seperate threads.
			#pragma omp parallel for
			for (int p = 0; p < 2; p++) {
				if (p == 0) { s1.run(); }
				else s2.run();
			}
		}
	}
}


