========================================================================
                     SIMULATION README: 4WJ FOLDER
========================================================================

This folder contains the result files from evolving a single controller
on the 4-way junction (4WJ):


========================================================================
		                  FOLDERS
========================================================================

Test graphs

Folder contains different investigations conducted on this controller.
More information for each can be found in the Project Report.


========================================================================
		                   FILES
========================================================================

4WJ_durations_evolved

Results from running each on the 4WJ using the evolved epiNet controller.
Each line is a test with each number in that line being a duration of
one of the cars to complete its route.
	

4WJ_durations_unimproved

Results from running each on the 4WJ using the round-robin controller.
Each line is a test with each number in that line being a duration of
one of the cars to complete its route.


main

main matlab script, run this to compare the two controllers graphically.


compareDurations

matlab function used to compare the two controller data sets.


methodology

details the exact methodology used to evolve the epiNet controller in
this section.