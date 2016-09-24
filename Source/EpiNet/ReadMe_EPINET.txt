========================================================================
                      SIMULATION README: EPINET FOLDER
========================================================================

This folder contains source files for the epiNet port.


========================================================================
		             Files
========================================================================

network

This is the top-level class of epiNet. After an instance is created
there are three main functions to interact with it. setInputs(), sets
the inputs into epiNet. executeNetwork() uses the inputs values to
determine new output values. getOutputs() gets the outputs from it.


calculations

This file includes some useful functions, that not only are used by
epiNet (and the evolution class), but can be used by you as well. 
These include randDouble() and randInt(), which return random random 
types in their range, and getMedian() and getMean() which can be which
average a vector<double>.


gene, epigeneticmolecule

These are epiNet objects and for a newcomer, are unlikely to be
needed to be understood or interacted with.
