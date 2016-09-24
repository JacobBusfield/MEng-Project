// Orignal Author: Dr. Alexander Turner
// ------------------------------------
// Ported to C++ by: Jacob Busfield
// Code structure, naming, etc. kept as close to the original as possible.
// Extra comments have been added in places to improve readability.
// Date: 08/05/2016
#include "stdafx.h"
#include "EpigeneticMolecule.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Constructs epigentic molecule with explicit settings.
EpigeneticMolecule::EpigeneticMolecule()
{
	//Where to look for inputs
	_inputsLocation = randDouble();

	//Distanct to look - this looks in the protein network
	_inputsProximity = randDouble(0.1, 0.3);
	_offset = randDouble(-1, 1);

	//slope between 0 and 20
	_slope = randDouble(0, 20);

	// this looks in the genetic network
	_outputProximity = randDouble(0.01, 0.02);
}

// Default destructor
EpigeneticMolecule::~EpigeneticMolecule() {}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

double EpigeneticMolecule::Execute(std::vector<double> inP, double slope, double offset){
	double res = 0;
	for (unsigned int i = 0; i < inP.size(); i++){ res += inP[i]; }
	return (1.0 / (1.0 + exp(-slope * (res - offset))));
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                  GETTER AND SETTER METHODS.                                    //
// -----------------------------------------------------------------------------------------------//

double EpigeneticMolecule::getSlope(){
	return _slope;
}

void EpigeneticMolecule::setSlope(double slope){
	_slope = slope;
}

double EpigeneticMolecule::getOffset(){
	return _offset;
}

void EpigeneticMolecule::setOffset(double offset){
	_offset = offset;
}

double EpigeneticMolecule::getInputsLocation(){
	return _inputsLocation;
}

void EpigeneticMolecule::setInputsLocation(double loc){
	_inputsLocation = loc;
}

double EpigeneticMolecule::getInputsProximity(){
	return _inputsProximity;
}

void EpigeneticMolecule::setInputsProximity(double prox){
	_inputsProximity = prox;
}

double EpigeneticMolecule::getOutputProximity(){
	return _outputProximity;
}

void EpigeneticMolecule::setOutputProx(double oProx){
	_outputProximity = oProx;
}
// -----------------------------------------------------------------------------------------------//
