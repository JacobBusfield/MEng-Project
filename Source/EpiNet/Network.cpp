// Orignal Author: Dr. Alexander Turner
// ------------------------------------
// Ported to C++ by: Jacob Busfield
// Code structure, naming, etc. kept as close to the original as possible.
// Extra comments have been added in places to improve readability.
// Date: 08/05/2016
#include "stdafx.h"
#include "Network.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Network constructor creates genome, epigenome and reference space.
Network::Network(int inNo, double inRatio, int outNo, double outRatio, int noOfGenes, 
				 int noOfEpgenetics){
	// Input/ Output reference space set in the constructor (no longer seperatly).
	// These will determine size of reference space genes can interact in.
	setInputSpace(inNo, inRatio);
	setOutputSpace(outNo, outRatio);

	// Store attributes locally
	_inNo = inNo;
	_inRatio = inRatio;
	_outNo = outNo;
	_outRatio = outRatio;
	_noOfGenes = noOfGenes;
	_noOfEpgenetics = noOfEpgenetics;
	
	// Create Genome and epiGenome.
	for (int i = 0; i < (noOfGenes); i++){ Genome.push_back(Gene()); }
	for (int i = 0; i < noOfEpgenetics; i++) { EpiGenome.push_back(EpigeneticMolecule()); }
}

// Default destructor
Network::~Network() {}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// The inputs are taken and mapped to input genes. Using the epigenome internal genes are selected 
// which compute values from the output genes
void Network::ExecuteGeneticNetwork(){
	TranscribeGenes();
	
	std::vector<std::vector<int>> connections;
	std::vector<int> connection;
	const int size = ProteinNetwork.size();

	//Loops over each genes connections
	for (signed int i = 0; i < size; i++){
		for (signed int x = 0; x < size; x++){
			//This is to prevent a gene becoming connected to its self
			if (x != i){
				//If the genes input is greate than the other genes iput minus its proximity 
				if (ProteinNetwork[x].getAlphaIdentifier() 
				  >(ProteinNetwork[i].getAlphaIdentifier() - ProteinNetwork[i].getAlphaProximity()))
				{
					//And the genes input is less than the other genes input plus its proxity
					if (ProteinNetwork[x].getAlphaIdentifier() 
					 < (ProteinNetwork[i].getAlphaIdentifier() 
						+ ProteinNetwork[i].getAlphaProximity()))
					{
						//Then they are connected! Horray!!!
						geneConnections.push_back(x);
					}
				}
			}
		}


		//Put those connections into the connections array for gene i 
		for (signed int x = 0; x < size; x++){
			if (geneConnections.size() > (unsigned)x) {
				connection.push_back(geneConnections[x]);
			}
		}
		connections.push_back(connection);
		connection.clear();
		geneConnections.clear();
	}

	// For each gene
	std::vector<double> results;
	for (signed int i = 0; i < size; i++){
		// Get the expressions of connected genes
		std::vector<double> expressions;
		
		// Get the weights of connected genes
		std::vector<double> weights;

		// Get the expressions and genes
		for (unsigned int x = 0; x < connections[i].size(); x++){
			expressions.push_back(ProteinNetwork[connections[i][x]].getExpression());
			weights.push_back(ProteinNetwork[connections[i][x]].getWeight());
		}

		// Add that up.
		double resultx = 0;
		for (unsigned int x = 0; x < expressions.size(); x++){
			if (weights.size() > x) {
				expressions[x] *= weights[x];
			}
		}
		for (unsigned int x = 0; x < expressions.size(); x++){
			resultx += expressions[x];
		}

		// Put it in the sigmoid function and update the genes epression value
		results.push_back (1.0 / 
			(1.0 + exp(-ProteinNetwork[i].getSlope() * (resultx - ProteinNetwork[i].getOffset()))));

	}

	for (int i = 0; i < size; i++){
		ProteinNetwork[i].setExpression(results[i]);
	}
}

// Sets inputs for epiNet. Extra inputs are ignored.
void Network::setInputs(std::vector<double> inputs){
	for (unsigned int i = 0; i < ProteinNetwork.size(); i++){
		for (unsigned int x = 0; x < inputPartitions.size(); x++){
			if (ProteinNetwork[i].getInputSelect() < inputPartitions[x]){
				if (x < inputs.size()) {
					ProteinNetwork[i].setExpression(inputs[x]);
					break;
				}
			}
		}
	}
}

// Gets outputs from epiNet.
std::vector<double> Network::getOutputs(int howmany){
	std::vector<double> outputs;
	std::vector<double> finout;

	for (unsigned int i = 0; i < ProteinNetwork.size(); i++){
		outputs.push_back(ProteinNetwork[i].getOutputSelect());
	}

	for (int x = 0; x < howmany; x++)
	{
		for (unsigned int i = 0; i < ProteinNetwork.size(); i++){
			auto max = std::max_element(std::begin(outputs), std::end(outputs));
			if (ProteinNetwork[i].getOutputSelect() == *max){
				finout.push_back(ProteinNetwork[i].getExpression());
				outputs[i] = (double)-INFINITY;
				i = -1;
				break;
			}
		}
	}

	return finout;
}

Gene Network::copyGeneValues(Gene gene){
	Gene NewGene;

	NewGene.setExpression(gene.getExpression());
	NewGene.setAlphaIdentifier(gene.getAlphaIdentifier());
	NewGene.setAlphaProximity(gene.getAlphaProximity());
	NewGene.setOffset(gene.getOffset());
	NewGene.setSlope(gene.getSlope());
	NewGene.setWeight(gene.getWeight());
	NewGene.setInputSelect(gene.getInputSelect());
	NewGene.setOutputSelect(gene.getOutputSelect());
	NewGene.setID(gene.getID());

	return NewGene;
}
EpigeneticMolecule Network::copyEpiValues(EpigeneticMolecule epi){
	EpigeneticMolecule epigentic;
	epigentic.setInputsLocation(epi.getInputsLocation());
	epigentic.setInputsProximity(epi.getInputsProximity());
	epigentic.setOffset(epi.getOffset());
	epigentic.setSlope(epi.getSlope());
	epigentic.setOutputProx(epi.getOutputProximity());
	return epigentic;
}

void Network::addGene(Gene G){
	Gene NewGene = copyGeneValues(G);
	Genome.push_back(NewGene);
}

void Network::removeGene(){
	if (Genome.size()) {
		Genome.erase(Genome.begin() + randInt(0, Genome.size()));
	}
}

void Network::removeGene(int y){
	if (Genome.size() > (unsigned)y) {
		Genome.erase(Genome.begin() + y);
	}
}

void Network::addEpigeneticMolecule(EpigeneticMolecule E){
	EpigeneticMolecule NewEpi = copyEpiValues(E);
	EpiGenome.push_back(NewEpi);
}

void Network::removeEpigeneticMolecule(){
	if (!EpiGenome.size()) {
		EpiGenome.erase(EpiGenome.begin() + randInt(0, EpiGenome.size()));
	}
}

Network Network::mutate(double percentage) {

	/* Initiliase random new network. From this the function will copy and paste any genes across
	   (that are not to be mutated) from the inputted network. Each parameter of each gene/epi has
	   the same chance of being changed so overall, by distribution of random numbers, the correct
	   ratio of parameters will be mutated. */
	Network n(getNumberOfInputs(), getRatioOfInputs(), getNumberOfOutputs(), getRatioOfOutputs(), 
			  getNumberOfGenes(), getNumberOfEpis());
	

	/* In each gene (in the genome) mutate ~ % of genes required */
	for (unsigned int i = 0; i < Genome.size(); i++) {
		if (randDouble() > percentage){
			n.Genome[i].setExpression(Genome[i].getExpression());
		}
		if (randDouble() > percentage){
			n.Genome[i].setAlphaIdentifier(Genome[i].getAlphaIdentifier());
		}
		if (randDouble() > percentage){
			n.Genome[i].setAlphaProximity(Genome[i].getAlphaProximity());
		}
		if (randDouble() > percentage){
			n.Genome[i].setOffset(Genome[i].getOffset());
		}
		if (randDouble() > percentage){
			n.Genome[i].setSlope(Genome[i].getSlope());
		}
		if (randDouble() > percentage){
			n.Genome[i].setWeight(Genome[i].getWeight());
		}
		if (randDouble() > percentage){
			n.Genome[i].setInputSelect(Genome[i].getInputSelect());
		}
		if (randDouble() > percentage){
			n.Genome[i].setOutputSelect(Genome[i].getOutputSelect());
		}
		if (randDouble() > percentage){
			n.Genome[i].setID(Genome[i].getID());
		}
	}

	/* In each epi molecule (in the epi-genome) mutate ~ % of epi molecules required */
	for (unsigned int i = 0; i < EpiGenome.size(); i++) {
		if (randDouble() > percentage){
			n.EpiGenome[i].setInputsLocation(EpiGenome[i].getInputsLocation());
		}
		if (randDouble() > percentage){
			n.EpiGenome[i].setInputsProximity(EpiGenome[i].getInputsProximity());
		}
		if (randDouble() > percentage){
			n.EpiGenome[i].setOffset(EpiGenome[i].getOffset());
		}
		if (randDouble() > percentage){
			n.EpiGenome[i].setSlope(EpiGenome[i].getSlope());
		}
		if (randDouble() > percentage){
			n.EpiGenome[i].setOutputProx(EpiGenome[i].getOutputProximity());
		}
	}
	return n;
}

Network Network::crossover(Network in, unsigned int points)
{
	Network n = in;

	// split it how many if statements in genome/ epi to pass before switching.
	unsigned int genomeSplit = (9 * Genome.size()) / (points + 1);
	unsigned int epiSplit = (5 * Genome.size()) / (points + 1);
	unsigned int count = 0;
	bool cross = false;

	/* Go through genome, at each point cross from overwriting genome to not.*/
	for (unsigned int i = 0; i < Genome.size(); i++) {
		for (int j = 0; j < 9; j++) {
			if (i < Genome.size()) {
				if (cross) {
					switch (j) {
					case 0:
						n.Genome[i].setExpression(Genome[i].getExpression());
						break;
					case 1:
						n.Genome[i].setAlphaIdentifier(Genome[i].getAlphaIdentifier());
						break;
					case 2:
						n.Genome[i].setAlphaProximity(Genome[i].getAlphaProximity());
						break;
					case 3:
						n.Genome[i].setOffset(Genome[i].getOffset());
						break;
					case 4:
						n.Genome[i].setSlope(Genome[i].getSlope());
						break;
					case 5:
						n.Genome[i].setWeight(Genome[i].getWeight());
						break;
					case 6:
						n.Genome[i].setInputSelect(Genome[i].getInputSelect());
						break;
					case 7:
						n.Genome[i].setOutputSelect(Genome[i].getOutputSelect());
						break;
					case 8:
						n.Genome[i].setID(Genome[i].getID());
						break;
					}
				}
				if (genomeSplit < count++) {
					cross = !cross;
					count = 0;
				}
			}
		}
	}

	count = 0;
	cross = false;
	/* Go through epi, at each point cross from overwriting epi to not.*/
	for (unsigned int i = 0; i < EpiGenome.size(); i++) {
		for (int j = 0; j < 5; j++) {
			if (i < Genome.size()) {
				if (cross) {
					switch (j) {
					case 0:
						n.EpiGenome[i].setInputsLocation(EpiGenome[i].getInputsLocation());
					case 1:
						n.EpiGenome[i].setInputsProximity(EpiGenome[i].getInputsProximity());
					case 2:
						n.EpiGenome[i].setOffset(EpiGenome[i].getOffset());
					case 3:
						n.EpiGenome[i].setSlope(EpiGenome[i].getSlope());
					case 4:
						n.EpiGenome[i].setOutputProx(EpiGenome[i].getOutputProximity());
					}

				}
				if (epiSplit < count++) {
					cross = !cross;
					count = 0;
				}
			}
		}
	}
	return n;
}


// Saves network to a text file (specified by name).
void Network::printNetwork(std::string name){
	std::ofstream file(name);

	if (file.is_open()) {
		file << "###in no., in ratio, out no., out ratio, gene no, Epgenetics no.###\n";
			file << getNumberOfInputs() << "\n";
			file << getRatioOfInputs() << "\n";
			file << getNumberOfOutputs() << "\n";
			file << getRatioOfOutputs() << "\n";
			file << getNumberOfGenes() << "\n";
			file << getNumberOfEpis() << "\n";

		file << "###########Genome#########\n";
		for (unsigned int i = 0; i < Genome.size(); i++){
			file << "*\n";
			Gene g = copyGeneValues(Genome[i]);

			file << g.getExpression() << "\n";
			file << g.getAlphaIdentifier() << "\n";
			file << g.getAlphaProximity() << "\n";
			file << g.getOffset() << "\n";
			file << g.getSlope() << "\n";
			file << g.getWeight() << "\n";
			file << g.getInputSelect() << "\n";
			file << g.getOutputSelect() << "\n";
			file << g.getID() << "\n";

		}

		file << "###########Epigenome#########\n";

		for (unsigned int i = 0; i < EpiGenome.size(); i++){
			file << "*\n";
			EpigeneticMolecule e = copyEpiValues(EpiGenome[i]);
			file << e.getInputsLocation() << "\n";
			file << e.getInputsProximity() << "\n";
			file << e.getOffset() << "\n";
			file << e.getOutputProximity() << "\n";
			file << e.getSlope() << "\n";
		}
	}
}

// Creates a new network by loading in saved one from text file (path is location of this file).
Network Network::readInNetwork(std::string path){
	
	std::vector<Gene> Genome;
	std::vector<EpigeneticMolecule> Epi;
	int geneLineCount = 0;
	
	
	
	std::ifstream file(path);
	if (file.fail()){
		std::cout << "ERROR: can't find Network file, " << path << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}

	std::string line;
	std::vector<std::string> lines;

	while (std::getline(file, line)){
		lines.push_back(line);
	}

	// Print Network to console (UNCOMMENT to view).
	/*for (unsigned int q = 0; q < lines.size(); q++) {
		std::cout << lines[q] << std::endl;
	}*/

	int inNo, outNo, noOfGenes, noOfEpgenetics;
	double inRatio, outRatio;
	if (lines[0] == "###in no., in ratio, out no., out ratio, gene no, Epgenetics no.###") {
		inNo = (int)std::stod(lines[1]);
		inRatio = std::stod(lines[2]);
		outNo = (int)std::stod(lines[3]);
		outRatio = std::stod(lines[4]);
		noOfGenes = (int)std::stod(lines[5]);
		noOfEpgenetics = (int)std::stod(lines[6]);
	}
	else return NULL;

	Gene g;
	for (unsigned int i = geneLineCount; i < lines.size(); i++){
		if (lines[i] == "*"){
			g.setExpression(std::stod(lines[i + 1]));
			g.setAlphaIdentifier(std::stod(lines[i + 2]));
			g.setAlphaProximity(std::stod(lines[i + 3]));
			g.setOffset(std::stod(lines[i + 4]));
			g.setSlope(std::stod(lines[i + 5]));
			g.setWeight(std::stod(lines[i + 6]));
			g.setInputSelect(std::stod(lines[i + 7]));
			g.setOutputSelect(std::stod(lines[i + 8]));
			g.setID(std::stod(lines[i + 9]));
			Genome.push_back(g);
			i = i + 9;
		}

		if (lines[i] == "###########Epigenome#########"){
			geneLineCount = i;
			break;
		}
		//If we hit epigenome, cut it
	}

	EpigeneticMolecule e;
	for (unsigned int i = geneLineCount; i < lines.size(); i++){
		if (lines[i] == "*"){

			e.setInputsLocation(std::stod(lines[i + 1]));
			e.setInputsProximity(std::stod(lines[i + 2]));
			e.setOffset(std::stod(lines[i + 3]));
			e.setOutputProx(std::stod(lines[i + 4]));
			e.setSlope(std::stod(lines[i + 5]));

			Epi.push_back(e);
			i = i + 5;
		}
		//If we hit epigenome, cut it
	}

	Network A(inNo, inRatio, outNo, outRatio, noOfGenes, noOfEpgenetics);

	A.Genome.clear();
	for (unsigned int i = 0; i < Genome.size(); i++){
		A.Genome.push_back(Genome[i]);
	}

	A.EpiGenome.clear();
	for (unsigned int i = 0; i < Epi.size(); i++){
		A.EpiGenome.push_back(Epi[i]);
	}

	return A;
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                         GETTER METHODS.                                        //
// -----------------------------------------------------------------------------------------------//
double Network::getEpiProx(){
	if (EpiGenome.size()) {
		return EpiGenome[0].getOutputProximity();
	}
	else return NULL;
}

int Network::getGenomeLength(){
	return Genome.size();
}

int Network::getNumberOfInputs(void){
	return _inNo;
}

double Network::getRatioOfInputs(void){
	return _inRatio;
}

int Network::getNumberOfOutputs(void){
	return _outNo;
}

double Network::getRatioOfOutputs(void){
	return _outRatio;
}

int Network::getNumberOfGenes(void) {
	return _noOfGenes;
}

int Network::getNumberOfEpis(void){
	return _noOfEpgenetics;
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                         PRIVATE FUNCTIONS                                      //
// -----------------------------------------------------------------------------------------------//

// Certain genes, depending on which epigenetic molecules select, are copied into a protein network.
void Network::TranscribeGenes(){
	// Copy back protein network values to genome 
	for (unsigned int i = 0; i < ProteinNetwork.size(); i++){
		for (unsigned int x = 0; x < Genome.size(); x++){
			if (Genome[x].getID() == ProteinNetwork[i].getID()){
				Genome[x].setExpression(ProteinNetwork[i].getExpression());
			}
		}
	}

	// Each epigenetic molecule can select genes to be transcribed.
	for (unsigned int x = 0; x < EpiGenome.size(); x++){
		// Get the input to the epigentic molecules first
		for (unsigned int i = 0; i < ProteinNetwork.size(); i++){
			if (sqrt(pow(ProteinNetwork[i].getAlphaIdentifier()-EpiGenome[x].getInputsLocation(),2)) 
			    < EpiGenome[x].getInputsProximity())
			{
				inputs.push_back(ProteinNetwork[i].getExpression() * ProteinNetwork[i].getWeight());
			}
		}

		// Determine region of genes activated using epigenetic molecule's sigmoid
		double positionToLook = 
			 EpigeneticMolecule::Execute(inputs, EpiGenome[x].getSlope(), EpiGenome[x].getOffset());
		
		// Cycle through genome, copying selected genes to the protein network.
		for (unsigned int i = 0; i < Genome.size(); i++){
			if (sqrt(pow(Genome[i].getID()-positionToLook,2)) < EpiGenome[x].getOutputProximity()){
				bool isPresentInProtien = false;
				for (unsigned int j = 0; j < ProteinNetwork.size(); j++){
					if (ProteinNetwork[j].getID() == Genome[i].getID()){
						isPresentInProtien = true;
						genesToAddToProtienNetwork.push_back(
							Network::copyGeneValues(ProteinNetwork[j]));
						break;
					}
				}

				if (!isPresentInProtien) {
					genesToAddToProtienNetwork.push_back(Network::copyGeneValues(Genome[i]));
				}
			}
		}
		inputs.clear();
	}
	ProteinNetwork.clear();

	// Genes in the Protein Network.
	for (unsigned int i = 0; i < genesToAddToProtienNetwork.size(); i++){
		ProteinNetwork.push_back(Network::copyGeneValues(genesToAddToProtienNetwork[i]));
	}
	genesToAddToProtienNetwork.clear();
}


// Creates input reference space using number of inputs and input ratio.
void Network::setInputSpace(int inputs, double input_ratio){
	double partition = input_ratio / (inputs);
	std::vector<double> partitions;

	partitions.push_back(partition);

	for (int i = 1; i < inputs; i++){
		partitions.push_back(partitions[i - 1] + partition);
	}

	inputPartitions = partitions;
}

// Creates output reference space using number of outputs and output ratio.
void Network::setOutputSpace(int outputs, double output_ratio){
	double partition = output_ratio / (outputs);
	std::vector<double> partitions;

	partitions.push_back(partition);

	for (int i = 1; i < outputs; i++){
		partitions.push_back(partitions[i - 1] + partition);
	}

	outputPartitions = partitions;
}

// -----------------------------------------------------------------------------------------------//