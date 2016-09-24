========================================================================
                   SIMULATION README: SOURCE FOLDER
========================================================================

This folder contains 4 folders which split up the software into four
modules. Be aware, there are heavy independencies between these modules.


========================================================================
		             FOLDERS
========================================================================

_Top Level

Files that configure, and call the other modules. This includes
main.cpp, the point of entry for the program.
	

Agent Based Simulation

Files that create and run the agent-based simulation. Simulation.cpp
is the top level class for these files.


EpiNet

Code ported from Dr. Alex Turner's original C# implementation. 
network.cpp is the top level class.


Evolution

A single set of functions used to evolve epiNet on Agent-based 
simulation by using a GA.