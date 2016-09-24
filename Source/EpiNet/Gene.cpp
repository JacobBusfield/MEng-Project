// Orignal Author: Dr. Alexander Turner
// ------------------------------------
// Ported to C++ by: Jacob Busfield
// Code structure, naming, etc. kept as close to the original as possible.
// Extra comments have been added in places to improve readability.
// Date: 08/05/2016
#include "stdafx.h"
#include "Gene.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Constructs Gene with explicit settings.
Gene::Gene(){
	_expression = randDouble();
	_alphaIdentifier = randDouble();

	/* Very arbitary - this values was used because it fits well between 0 and 1 - which are the min and max of the reference space. */
	_alphaProximity = randDouble() / 7.5;

	/* Is this gene going to be an input, output or both? */
	_inputSelect = randDouble();
	_outputSelect = randDouble();

	/* Weights and offsets are set between -1 and 1 */
	_weight = randDouble(-1, 1);
	_offset = randDouble(-1, 1);

	/* slope between 0 and 20 */
	_slope = randDouble(0, 20);

	/* ID that epigeentics will use to transcribe the gene */
	_id = randDouble();
}

// Default deconstructor
Gene::~Gene() {}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                  GETTER AND SETTER METHODS.                                    //
// -----------------------------------------------------------------------------------------------//

double Gene::getExpression(void){
	return _expression;
}

void Gene::setExpression(double expression){
	_expression = expression;
}

double Gene::getAlphaIdentifier(void){
	return _alphaIdentifier;
}

void Gene::setAlphaIdentifier(double alphaIdentifier){
	_alphaIdentifier = alphaIdentifier;
}

double Gene::getAlphaProximity(void){
	return _alphaProximity;
}

void Gene::setAlphaProximity(double alphaProximity){
	_alphaProximity = alphaProximity;
}

double Gene::getInputSelect(void){
	return _inputSelect;
}

void Gene::setInputSelect(double input){
	_inputSelect = input;
}

double Gene::getOutputSelect(void){
	return _outputSelect;
}

void Gene::setOutputSelect(double output){
	_outputSelect = output;
}

double Gene::getWeight(void){
	return _weight;
}

void Gene::setWeight(double weight){
	_weight = weight;
}

double Gene::getSlope(void){
	return _slope;
}

void Gene::setSlope(double slope){
	_slope = slope;
}

double Gene::getOffset(void){
	return _offset;
}

void Gene::setOffset(double offset){
	_offset = offset;
}

double Gene::getID(void){
	return _id;
}

void Gene::setID(double id){
	_id = id;
}

// -----------------------------------------------------------------------------------------------//
