========================================================================
                  SIMULATION README: IMPORTED 4WJ FOLDER
========================================================================

This folder contains the result files from importing 4wj controller
and duplicating it on the traffic network (4WJX4).


========================================================================
		                   FILES
========================================================================

4WJX4_durations_evolved

Results from running each on the 4WJX4 using the imported 4WJ epiNet 
controllers. Each line is a test with each number in that line being a
duration of one of the cars to complete its route.
	

4WJX4_durations_unimproved

Results from running each on the 4WJX4 using round-robin controllers. 
Each line is a test with each number in that line being a duration of 
one of the cars to complete its route.


main

main matlab script, run this to compare the two controllers graphically.


compareDurations

matlab function used to compare the two controller data sets.