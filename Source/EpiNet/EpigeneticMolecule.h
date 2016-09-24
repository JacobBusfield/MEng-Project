// Orignal Author: Dr. Alexander Turner
// ------------------------------------
// Ported to C++ by: Jacob Busfield
// Code structure, naming, etc. kept as close to the original as possible.
// Extra comments have been added in places to improve readability.
// Date: 08/05/2016
#pragma once

#include "Calculations.h"
#include <vector>
#include <math.h>

// Class: EpigeneticMolecule
//
// The epigenetic molecule is used to straddle a genome to choose which genes are transcribed into
// a protein network.
class EpigeneticMolecule
{
public:
// Constructor:
	// Constructs epigentic molecule with explicit settings.
	EpigeneticMolecule();
	
	// Default destructor
	~EpigeneticMolecule();

	
// Public Functions:
	static double Execute(std::vector<double> inP, double slope, double offset);

	// Getter/ Setters:
	double getSlope();
	void setSlope(double slope);
	double getOffset();
	void setOffset(double offset);
	double getInputsLocation();
	void setInputsLocation(double loc);
	double getInputsProximity();
	void setInputsProximity(double prox);
	double getOutputProximity();
	void setOutputProx(double oProx);

private:
// Private Variables:
	double _inputsLocation;  // This uses the active genes to fina a location which needs
							 // transcribing.
	double _inputsProximity; // Gene's location in reference space and how far they have searched
							 // for connections.
	double _outputProximity; // How far to transcribe the genes in each direction
	
	// These values are useed update the gene expression value using the sigmoid function 
	double _slope;
	double _offset;
};

