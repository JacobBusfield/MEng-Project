========================================================================
                    SIMULATION README: SINGLE FOLDER
========================================================================

This folder contains the result files from evolving a single master
controller on the traffic network (4WJX4).


========================================================================
		                   FILES
========================================================================

4WJX4_durations_evolved

Results from running each on the 4WJX4 using the single master epiNet 
controller. Each line is a test with each number in that line being a
duration of one of the cars to complete its route.
	

4WJX4_durations_unimproved

Results from running each on the 4WJX4 using round-robin controllers. 
Each line is a test with each number in that line being a duration of 
one of the cars to complete its route.


4WJX4_durations_populations

Each line contains a single value. This value is the mean fitness value
of the champion in the GA's population for each generation.


main

main matlab script, run this to compare the two controllers graphically.


compareDurations

matlab function used to compare the two controller data sets.


evoProgression

matlab function used to display the data of how the controller evolved
over subsequent generations.


methodology

details the exact methodology used to evolve the epiNet controllers in
this section.