========================================================================
                     SIMULATION README: 3WJ FOLDER
========================================================================

This folder contains the result files from evolving a single controller
on the 3-way junction (3WJ):


========================================================================
		                 FILES
========================================================================

3wj_durations_evolved

Results from running each on the 3WJ using the evolved epiNet controller.
Each line is a test with each number in that line being a duration of
one of the cars to complete its route.
	

3wj_durations_unimproved

Results from running each on the 3WJ using the round-robin controller.
Each line is a test with each number in that line being a duration of
one of the cars to complete its route.


main

main matlab script, run this to compare the two controllers graphically.


compareDurations

matlab function used to compare the two controller data sets.


methodology

details the exact methodology used to evolve the epiNet controller in
this section.