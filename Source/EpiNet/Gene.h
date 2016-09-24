// Orignal Author: Dr. Alexander Turner
// ------------------------------------
// Ported to C++ by: Jacob Busfield
// Code structure, naming, etc. kept as close to the original as possible.
// Extra comments have been added in places to improve readability.
// Date: 08/05/2016
#pragma once
#include "Calculations.h"

// Class: Gene
//
// Gene holds information that can be used, by the network (in network.h) to connect multiple genes 
// together. These (protein) networks can be used to perfrom complex mathematical operations.
class Gene
{
public:
// Constructor:
	// Constructs Gene with explicit settings.
	Gene();
	// Default deconstructor
	~Gene();

// Public Functions:
	// Getter/ Setters:
	double getExpression(void);
	void setExpression(double expression);
	double getAlphaIdentifier(void);
	void setAlphaIdentifier(double alpha_identifier);
	double getAlphaProximity(void);
	void setAlphaProximity(double alpha_proximity);
	double getInputSelect(void);
	void setInputSelect(double input);
	double getOutputSelect(void);
	void setOutputSelect(double output);
	double getWeight(void);
	void setWeight(double weight);
	double getSlope(void);
	void setSlope(double slope);
	double getOffset(void);
	void setOffset(double offset);
	double getID(void);
	void setID(double id);

private:
// Private Variables:
	double _id; 		// Unique Gene ID.	
	double _expression; // This is the genes expression value, indicating its activity.

	// This is the genes location in the reference space and how far they search for connections.
	double _alphaIdentifier;
	double _alphaProximity;

	//Thes values are used to update the gene expression value using the sigmoid function 
	double _weight;
	double _slope;
	double _offset;

	// Input and output of gene.
	double _inputSelect;
	double _outputSelect;
};

