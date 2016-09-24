// Orignal Author: Dr. Alexander Turner
// ------------------------------------
// Ported to C++ by: Jacob Busfield
// Code structure, naming, etc. kept as close to the original as possible.
// Extra comments have been added in places to improve readability.
// Date: 08/05/2016
#pragma once
#include "EpigeneticMolecule.h"
#include "Gene.h"
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>    
#include <iostream>
#include <fstream>
#include <thread>         
#include <chrono>        
#include <iostream>
#include <sstream>


// Class: Network
//
// A Breif Summary
// a singular strand of genes, or genome, existing on a linear scale between [0,1]. Straddling the 
// genome exists aFixed-width object, which mimics the functionality of chromatin, that selects 
// which portion of the genome is transcribed and thus which genes are expressed. In this model,
// each active gene synthesises a single artifical protein. Within a single epigenetic analogue, 
// multiple connections are formed between proteins creating a protein network. The connections
// between these proteins are pre-determined by the properties of their parent gene in a methodology
// referred to as the reference space. Connections act as either inputs to, or outputs from, either
// other proteins or the inputs and outputs of the entire epiNet structure. The protein: sums its 
// inputs with pre-determined weights; converts the result through an internal sigmoid function; 
// before outputting the value through its output connections. The larger, combined network can 
// therefore perform complex mathematical operations between the inputs and outputs of the system.
// The epigenetic molecule then uses the protein network, and its own internal sigmoid function, to
// determine where to subsequently move. This allows, as the original protein network has been 
// destroyed, for a new combination of genes to be expressed and thus a new protein network to be
// created. This in turn, changes the mathematical operation performed between the inputs and 
// outputs of epiNet depending on the local state of the system.
//
// This class is the top level of epiNet. It base functionality once created is:
// 		1. Set its inputs using setInputs().
//		2. Execute the network using ExecuteGeneticNetwork().
//		3. Get outputs from the network using getOutputs().
//
// Functions also exist to assist the evolution of epiNet through an evolutionary algorithm, such as
// its mutate and crossover functions. And the network can be printed and loaded from a text file.
//
// This class has a few changes from the original. The most notable - there is no set reference 
// space functions and no static variables. Instead the constructor is used to specify how many and 
// the ratio of inputs and outputs.
class Network
{
public:
// Constructor:
	// Network constructor creates genome, epigenome and reference space.
	Network(int inNo = 0, double inRatio = 0, int outNo = 0, double outRatio = 0, int noOfGenes=0,
			int noOfEpgenetics = 0);
	// Default destructor.
	~Network();

// Public Functions:
	// Certain genes, depending on which epigenetic molecules select, are copied into a protein
	// network.
	void ExecuteGeneticNetwork();

	// Functions to interact with epiNet's IOs
	void setInputs(std::vector<double> inputs);
	std::vector<double> getOutputs(int howmany);
	
	// Functions to copy objects (kept for legacy).
	Gene copyGeneValues(Gene gene);
	EpigeneticMolecule copyEpiValues(EpigeneticMolecule epi);

	// Functions to manipulate epiNet's genetic structure.
	void addGene(Gene G);
	void removeGene();
	void removeGene(int y);
	void addEpigeneticMolecule(EpigeneticMolecule E);
	void removeEpigeneticMolecule();

	// Functions to assist with evolving epiNet.
	Network mutate(double percentage);
	Network crossover(Network in, unsigned int points);

	// Save to, and load from, text files specified by name and path respectively.
	void printNetwork(std::string name);
	static Network readInNetwork(std::string path);

	// Getters
	double getEpiProx();
	int getGenomeLength();
	int getNumberOfInputs(void);
	double getRatioOfInputs(void);
	int getNumberOfOutputs(void);
	double getRatioOfOutputs(void);
	int getNumberOfGenes(void);
	int getNumberOfEpis(void);


private:
// Private Functions:
	// The input/ output space is now set in the constructor so these variables have become private.
	void setInputSpace(int inputs, double input_ratio);
	void setOutputSpace(int outputs, double output_ratio);	
	
	// Certain genes, depending on which epigenetic molecules select, are copied into a protein 
	// network.
	void TranscribeGenes();

// Private Variables:
	std::vector<Gene> Genome;				   // This is the genetic network -a collection of genes
	std::vector<EpigeneticMolecule> EpiGenome; // This is the epigenetic network - a collection of 
											   // epigenetic molecules
	std::vector<Gene> ProteinNetwork;		   // This is the list of transcribed genes
	std::vector<Gene> genesToAddToProtienNetwork; // Temp variable used to form protein network.
	std::vector<double> inputs;				   // Copy of the inputs to epiNet. This is changed when
											   // setInputs() is called.
	std::vector<int> geneConnections;		   // Used to determine which genes are connected in 
											   // the protein networks.
	
	// Used to partition the reference space.
	std::vector<double> inputPartitions;
	std::vector<double> outputPartitions;

	// (New) variables store what this instance of epiNet's size is.
	int _inNo, _outNo, _noOfGenes, _noOfEpgenetics; 
	double _inRatio, _outRatio;						
};

