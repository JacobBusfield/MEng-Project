// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "simulation.h"


// -----------------------------------------------------------------------------------------------//
//                                           CONSTRUCTOR                                          //
// -----------------------------------------------------------------------------------------------//

// Create simulation with given design and test. Systems specified as using epiNet to control 
// aspects of desired systems will also be formed based upon the map file and the passed in vector
// of epiNet instances.
Simulation::Simulation(std::string pathDesign, std::string pathMap, std::string pathTest,
					   std::vector<Network*> epiNetworks){
	n = epiNetworks;

	// The inputs and outputs from the epiNet instances are mapped to input and output vectors.
	// These vectors will later use the map file to map the vector to systems. Hence whenever epiNet
	// is updated, these vectors will auto update, and thus the systems will be to.
	for (unsigned int i = 0; i < n.size(); i++) {
		std::vector<double> temp(n[i]->getNumberOfInputs());
		inN.push_back(temp);
	}

	for (unsigned int i = 0; i < n.size(); i++) {
		std::vector<double> temp(n[i]->getNumberOfOutputs());
		outN.push_back(temp);
	}

	// Save path locations
	designPath = pathDesign;
	mapPath = pathMap;
	testPath = pathTest;

	// Load image, once, to be used to texture cars.
	if (!texture_car.loadFromFile("Images/car.png")) {
		std::cout << "ERROR: Cannot load Images/car.png.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}
}

// Create simulation with given design and test. All Systems will use round-robin configuration.
Simulation::Simulation(std::string pathDesign, std::string pathTest){
	// Save path locations
	designPath = pathDesign;
	testPath = pathTest;
	
	// Load image, once, to be used to texture cars.
	if (!texture_car.loadFromFile("Images/car.png")) {
		std::cout << "ERROR: Cannot load Images/car.png.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}
}

// Default destructor
Simulation::~Simulation() {}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                        PUBLIC FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// Simulation can be run with or without GUI. If GUI is specified a new (SFML) render window is
// opened and a simulation graphically displayed within it. This simulation's speed can be 
// controlled using the arrow keys, and exited. If no GUI the simulation runs as fast as
// possible. This function only returns after the simulation has completed.
void Simulation::run(bool showGUI){
	// Road contains the pointer 'this' to make connections. Therefore if its overaching structure
	// is moved, the this pointer is now looking at empty space. Therefore simulation objects are 
	// made on each run iteration so it matters not if the User may attempt to move them. This is
	// somewhat wasteful of resources but stands as the best course of action without overhauling 
	// the whole simulation code structure.
	std::vector<Location> l;
	std::vector<System> s;
	std::vector<Road> r;
	std::vector<Car> c;
	
	// Parse config files, catch any errors (by exiting programming).
	try { parseSim(&l, &s, &r); }
	catch (...) {
		std::cout << "ERROR: in sim file.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}
	if (mapPath != "na") {
		try { parseMap(&s); }
		catch (...) {
			std::cout << "ERROR: in map file.\n";
			std::this_thread::sleep_for(std::chrono::seconds(3));
			exit(EXIT_FAILURE);
		}
	}
	try { parseTest(&l, &s, &c); }
	catch (...) {
		std::cout << "ERROR: in test file.\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}
	

	// SHOW SIM DEPENDING ON REQUIREMENTS.
#if GUI == true
	if (showGUI == true) {
		runWindow(&l, &s, &r, &c);
	}
	else {
#endif
		runNoWindow(&l, &s, &r, &c);
#if GUI == true
	}
#endif

	// Update MOE.
	resetMOE();
	for (unsigned int i = 0; i < c.size(); i++) {
		// If car didn't finish, sim must have timed-out. Therefore add maximum length car could 
		// possibly go for (a 'valid' solution, if it exists, will never exceed this time - that
		// would be impossible). Doing this ensures timeouts are not heavily disfavoured.
		if (c[i].followingRoute()) { addCarDuration(SIM_TIMEOUT); }
		else if (c[i].routeDuration() > 0) { addCarDuration(c[i].routeDuration()); }
	}
}

// -----------------------------------------------------------------------------------------------//




// -----------------------------------------------------------------------------------------------//
//                                       PRIVATE FUNCTIONS                                        //
// -----------------------------------------------------------------------------------------------//

// EpiNet controllers require updating to process inputs into new outputs. Inputs and outputs have 
// already been mapped by constructor to inN and outN so by calling this function each controller 
// will automatically recieve updated commands.
void Simulation::updateEpi(void){
	// Send Inputs to network.
	for (unsigned int i = 0; i <inN.size(); i++) { n[i]->setInputs(inN[i]); }

	// Execute Networks for each.
	for (unsigned int i = 0; i < n.size(); i++) { n[i]->ExecuteGeneticNetwork(); }

	// Get outputs from network.
	for (unsigned int i = 0; i < outN.size(); i++) {
		std::vector<double> temp(n[i]->getOutputs(outN[i].size()));
		for (unsigned int j = 0; j < temp.size(); j++) { outN[i][j] = temp[j]; }
		temp.clear();
	}
}

// Runs simulation with GUI. Uses loop that updates simulation objects, draws them and displays them
// on a render window. repeats until simulation completes.
void Simulation::runWindow(std::vector<Location> * l, std::vector<System> * s,
						   std::vector<Road> * r, std::vector<Car> * car){
	// --- INIT --- //
	// Varuables to track time, and render window title.
	std::string command;
	if (simSpeed == FAST) { 
		command = "Simulation: FAST SPEED  (Down Key to Decrease Speed)"; 
	}
	else if (simSpeed == EXACT) { 
		command = "Simulation: EXACT SPEED (Up Key to Increase Speed, Down to pause)"; 
	}
	double totalTime = 0;

	// SFML Window
	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTIALIAS_LEVEL;
	sf::RenderWindow renderWindow(
		sf::VideoMode(WIDTH, HEIGHT), "Simulation(0): "+command, sf::Style::Default, settings
	);
	sf::Event event;
	event.key;

	// Return Window to same place as prior run.
	if (firstRun == true) {
		firstRun = false;
		rwPos = renderWindow.getPosition();
		rwSize = renderWindow.getSize();
	}
	else {
		renderWindow.setPosition(rwPos);
		renderWindow.setSize(rwSize);
	}

	// Clock
	sf::Clock clock;
	clock.restart();
	timeout.restart();

	
	// --- MAIN SIM LOOP --- //
	bool end = false;
	while (end == false) {
		
		// --- CLOCK --- //
		if (simSpeed == EXACT) {
			// If there is a request to have simulation run at real-time wait for the current frame 
			// until in reaches the predefined (config.h) frame time length.
			if (clock.getElapsedTime().asSeconds() < FRAME_TIME) {
				std::this_thread::sleep_for(std::chrono::milliseconds(
							   (int)(FRAME_TIME * 1000 - clock.getElapsedTime().asMilliseconds())));
			}
			clock.restart();
		}
		if (simSpeed != STOPPED) {
			totalTime += FRAME_TIME;
		}


		// --- EVENTS --- //
		// Check for all the events that occured since the last frame.
		while (renderWindow.pollEvent(event)) {
			switch (event.type)
			{
				// Window closed
				case sf::Event::Closed:
					// If window is closed in pause mode, next sim will start in pause mode.
					// This isn't desired so explitily change this here.
					if (simSpeed == STOPPED) {
						simSpeed = EXACT;
					}
					rwPos = renderWindow.getPosition();
					rwSize = renderWindow.getSize();
					renderWindow.close();
					break;

				// Key pressed
				case sf::Event::KeyPressed:
					// Up increases simulation speed.
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
						if (simSpeed == EXACT) {
							simSpeed = FAST;
							command = "Simulation: FAST SPEED  (Down Key to Decrease Speed)";
						}
						else if (simSpeed == STOPPED) {
							simSpeed = EXACT;
							command = 
								"Simulation: EXACT SPEED (Up Key to Increase Speed, Down to pause)";
						}
					}
					// Down decreases simulation speed.
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
						if (simSpeed == FAST) {
							simSpeed = EXACT;
							command = 
								"Simulation: EXACT SPEED (Up Key to Increase Speed, Down to pause)";
						}
						else if (simSpeed == EXACT) {
							simSpeed = STOPPED;
							command = "Simulation: PAUSED      (Up Key to Increase Speed)";
						}
					}
					break;

				// Default
				default:
					break;
			}
		}
		if (!renderWindow.isOpen()) { end = true; }

		// Update window's title
		std::string titleString = "Simulation( " + std::to_string(totalTime) + "): " + command;
		renderWindow.setTitle(titleString);


		if (simSpeed != STOPPED) {
			// --- SIMULATION LOGIC --- //
			if (epiCounter == EPI_UPDATE_TICKS) {
				updateEpi();
				epiCounter = 0;
			}
			else {
				epiCounter++;
			}
			for (unsigned int i = 0; i < l->size(); i++) { l->at(i).update(); }
			for (unsigned int i = 0; i < s->size(); i++) { s->at(i).update(); }
			for (unsigned int i = 0; i < r->size(); i++) { r->at(i).update(); }

			// clock based on frames is used to measure how long car is running therefore must be 
			// updated each frame.
			for (unsigned int i = 0; i < car->size(); i++) { car->at(i).updateRouteClock(); }


			// ---  DRAW --- //
			renderWindow.clear();
			for (unsigned int i = 0; i < r->size(); i++) { renderWindow.draw(r->at(i)); }
			for (unsigned int i = 0; i < l->size(); i++) { renderWindow.draw(l->at(i)); }
			for (unsigned int i = 0; i < s->size(); i++) { renderWindow.draw(s->at(i)); }

			for (unsigned int i = 0; i < car->size(); i++) {
				if (car->at(i).getDone() == true) {}
				else { renderWindow.draw(car->at(i)); }
			}
			renderWindow.display();


			// --- TERMINATION CONDITION --- //
			// Condition prevents needless termination checking, and prevents 'being set back to 
			// true' bug.
			if (end == false) { 
				// If any car is following a route, don't end.
				end = true;
				for (unsigned int i = 0; i < car->size(); i++) {
					if (car->at(i).followingRoute()) {
						end = false;
					}
				}
				// If simulation has been run longer than desited then 'Time-out'.
				timeout.updateClock();
				if (timeout.getElapsedTime() > SIM_TIMEOUT) {
					end = true;
				}
			}
		}
	}
	// If window is still open, grab coords to use for next run.
	if (renderWindow.isOpen()) {
		rwPos = renderWindow.getPosition();
		rwSize = renderWindow.getSize();

		renderWindow.close();
	}
}

// Runs simulation without a GUI. Runs simulation logic as fast as possible.
void Simulation::runNoWindow(std::vector<Location> * l, std::vector<System> * s, 
							 std::vector<Road> * r, std::vector<Car> * car){
	// INIT
	timeout.restart();

	
	// --- MAIN SIM LOOP --- //
	bool end = false;
	while (end == false) {

		// --- SIMULATION LOGIC --- //
		if (epiCounter == EPI_UPDATE_TICKS) {
			updateEpi();
			epiCounter = 0;
		}
		else {
			epiCounter++;
		}
		for (unsigned int i = 0; i < l->size(); i++) { l->at(i).update(); }
		for (unsigned int i = 0; i < s->size(); i++) { s->at(i).update(); }
		for (unsigned int i = 0; i < r->size(); i++) { r->at(i).update(); }

		// clock based on frames is used to measure how long car is running therefore must be 
		// updated each frame.
		for (unsigned int i = 0; i < car->size(); i++) { car->at(i).updateRouteClock(); }


		// --- TERMINATION CONDITION --- //
		// If any car is following a route, don't end.
		end = true;
		for (unsigned int i = 0; i < car->size(); i++) {
			if (car->at(i).followingRoute()) {
				end = false;
			}
		}
		// If simulation has been run longer than desired then 'Time-out'.
		timeout.updateClock();
		if (timeout.getElapsedTime() > SIM_TIMEOUT) {
			end = true;
		}
	}
}

// Function used to parse a simulation design files.
void Simulation::parseSim(std::vector<Location> * l, std::vector<System> * s, std::vector<Road> *r){
	// Open design file
	std::ifstream file(designPath);
	if (file.fail())
	{
		std::cout << "ERROR: can't find sim file, " << designPath << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}
	
	// Load file into vector of lines.
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)) { lines.push_back(line); }
	
	// Parse file:
	//
	// Encountered Location?
	//	- next 2 lines are position of it.
	//
	// Encountered System?
	//	- next 2 lines are position of it.
	// 
	// Encountered Road?
	//  - next two lines are locations/systems to connect.
	std::vector<Node*> connections;
	for (unsigned int i = 0; i < lines.size(); i++) {
		if (lines[i] == "Location") {
			double x = (std::stod(lines[i + 1]) * (WIDTH - RADIUS * 2)) + RADIUS;
			double y = (std::stod(lines[i + 2]) * (HEIGHT - RADIUS * 2)) + RADIUS;
			l->push_back(Location(sf::Vector2f((float)x, (float)y)));
			i += 2;
		}
		else if (lines[i] == "System") {
			double x = (std::stod(lines[i + 1]) * (WIDTH - RADIUS * 2)) + RADIUS;
			double y = (std::stod(lines[i + 2]) * (HEIGHT - RADIUS * 2)) + RADIUS;
			s->push_back(System(sf::Vector2f((float)x, (float)y)));
			i += 2;
		}
		else if (lines[i] == "Road") {
			r->push_back(Road((const float)(std::stod(lines[i+1]))));
			if (lines[i+2] == "Location") {
				connections.push_back(&l->at(((int)std::stod(lines[i + 3])) - 1));
			}
			else if (lines[i + 2] == "System") {
				connections.push_back(&s->at(((int)std::stod(lines[i + 3])) - 1));
			}
			
			if (lines[i + 4] == "Location") {
				connections.push_back(&l->at(((int)std::stod(lines[i + 5])) - 1));
			}
			else if (lines[i + 4] == "System") {
				connections.push_back(&s->at(((int)std::stod(lines[i + 5])) - 1));
			}
			i += 5;
		}
	}
	
	// Connections can only be made after all roads have been constructed.
	unsigned int rPos = 0;
	for (unsigned int i = 0; i < connections.size(); i += 2) {
		r->at(rPos).connect(connections[i], connections[i+1]);
		rPos++;
	}
}

// Function used to parse map files.
void Simulation::parseMap(std::vector<System>* s){
	// Open map file
	std::ifstream file(mapPath);
	if (file.fail())
	{
		std::cout << "ERROR: can't find map file, " << mapPath << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}

	// Load file into vector of lines.
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)){ lines.push_back(line); }

	// Parse file:
	//
	// Encountered System?
	//	- next line is system number to apply mapping
	// 	- End not encountered?
	//  - - Input encountered?
	//  - - - next line is index number to take from controller (whos no. is 2 lines below that).
	std::vector<Node*> connections;
	for (unsigned int i = 0; i < lines.size(); i++) {
		if (lines[i] == "System") {
			unsigned int systemNo = (unsigned int)std::stod(lines[i + 1])-1;
			std::vector<double*> inputs;
			std::vector<double*> outputs;
			i += 2;
			while (i < lines.size()) {
				if (lines[i] == "end") {
					break;
				}
				// Input to system is output from network. 
				else if (lines[i] == "Input") {
					inputs.push_back(&outN[(unsigned int)std::stod(lines[i + 3])-1]
										  [(unsigned int)std::stod(lines[i + 1])-1]);
					i += 3;
				}
				// Output from system is input to network. 
				else if (lines[i] == "Output") {
					outputs.push_back(&inN[(unsigned int)std::stod(lines[i + 3])-1]
										  [(unsigned int)std::stod(lines[i + 1])-1]);
					i += 3;
				}
				i++;
			}
			// Complete mapping.
			s->at(systemNo).addController(inputs, outputs);
		}
	}
}

// Function used to parse test files.
void Simulation::parseTest(std::vector<Location>* l, std::vector<System> * s, std::vector<Car> * c){
	// Open File 
	std::ifstream file(testPath);
	if (file.fail())
	{
		std::cout << "ERROR: can't find test file, " << testPath <<"\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}

	// Load file into vector of lines.
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)){ lines.push_back(line); }


	//Parse file to local variables
	//
	// This is a strange method that will take a while to understand...
	// Essentially the test file first states a location, and then 1 of 3 operations that occur 
	// there,
	// 		Route - Specifies the route to be taken from location. So to system 1, to Location 3 for
	//			    example. And the number of cars to be spawned with this route.
	// 		Spawn - Spawns n cars, with no route.
	// 		Delay - Specifies how many car spawns should be skipped (adding a delay).
	//	
	// Now the weirdness comes from the fact the cars cannot be added to the c vector intertively as
	// the class moves the data too much causing problems with some array addresses. Annoying. So 
	// a vector is built, and then added in one go. Only then is this added to the locations for
	// spawning. To simplify that - cars must be collated into a single vector and then added to the
	// simulation in one group.
	//
	// So the operations remaing in this function are as such:
	// 		2. Parse data into the toAdd object.
	// 		3. Add parsed cars in toAdd.c to the car list passed in: c.
	// 		4. Spawn cars at the location passed in: l.
	// Other nuances also exist. Such as delays which are added to toAdd.d and then passed to the 
	// location spawn as NULL. How this is done is best seen by inspection of the code.
	lCars toAdd;
	std::vector<Car> cars;
	std::vector<unsigned int> delays;
	for (unsigned int i = 0; i < lines.size(); i++) {
		// First determine location, either random or fixed, to spawn cars.
		if (lines[i] == "Location") {
			if (lines[i + 1] == "Rand") {
				int locationIndex = randInt(1, (int)ceil(std::stod(lines[i + 2])));
				toAdd.lNo.push_back(locationIndex);
				i += 1;
			}
			else { toAdd.lNo.push_back((unsigned int)ceil(std::stod(lines[i + 1])) - 1); }

			// We have just switch to a new location. If the first time we'ce switched then this
			// code is ignored. If this it not the first then cars will exist in temporary storage 
			// for that last location. This is a good time to move them to more permanent storage.
			if (!cars.empty()) {
				toAdd.c.push_back(cars);
				cars.clear();

				toAdd.d.push_back(delays);
				delays.clear();
			}
			i++;
		}
		// Spawn cars (they have NO route) at aforementioned location
		else if (lines[i] == "Spawn") {
			// Spawn a number of cars at the location.
			for (unsigned int j = 0; j < (unsigned int)std::stod(lines[i + 1]); j++) {
				cars.push_back(Car(&texture_car));
			}
			i++;
		}
		// Spawn cars (they have a route) at aforementioned location
		else if (lines[i] == "Route") {
			i++;
			Car car(&texture_car);			// Single car is given route, then copied (if needed).
			car.setColor(sf::Color::Green); // Cars with a route are green.
			bool parsingRoute = true;
			while (parsingRoute == true) {
				// Cars can visit either systems...
				if (lines[i] == "System") {
					car.addRoute(&s->at((unsigned int)std::stod(lines[i + 1]) - 1));
					i += 2;
				}
				// ... Or locations.
				else if (lines[i] == "Location") {
					if (lines[i + 1] == "Rand") {
						int locationIndex = randInt(1, (int)ceil(std::stod(lines[i + 2])));
						car.addRoute(&l->at(locationIndex - 1));
						i += 3;
					}
					else {
						car.addRoute(&l->at((unsigned int)std::stod(lines[i + 1]) - 1));
						i += 2;
					}
				}
				// The number of cars to be spawned is stated.
				else if (lines[i] == "Number") {
					// Either random amount, limited by given number.
					if (lines[i + 1] == "Rand") {
						int toSpawn = randInt(0, (int)std::stod(lines[i + 2]));
						for (int j = 0; j < toSpawn; j++) {
							cars.push_back(car);
						}
						i += 2;
					}
					// or fixed amount.
					else {
						for (unsigned int j = 0; j < (unsigned int)std::stod(lines[i + 1]); j++) {
							cars.push_back(car);
						}
						i++;
					}
					// When spawn number is specified - route declaration has finished.
					parsingRoute = false;
				}
				// Blank line found, just skip it. 
				// If end of file is found before route has completed throw an error.
				else {
					i++;
					if (i > lines.size()) {
						std::cout << 
							"ERROR: can't find end of Route information in, " << mapPath << "\n";
						std::this_thread::sleep_for(std::chrono::seconds(3));
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		// Add a delay between sets of spawned cars at a location. These will eventually become NULL
		// entries in a location's spawn list.
		else if (lines[i] == "Delay") {
			if (lines[i + 1] == "Rand") {
				int toWait = randInt(0, (int)std::stod(lines[i + 2]));
				for (int j = 0; j < toWait + 1; j++) {
					delays.push_back(cars.size()); // get place in car chain to add the delay.
				}
				i += 2;
			}
			else {
				// Need n+1 empty cars as need to block current spawn, and spawn while car moves 
				// away. This take n+1 delays.
				for (unsigned int j = 0; j < (unsigned int)std::stod(lines[i + 1]) + 1; j++) {
					delays.push_back(cars.size());
				}
				i++;
			}
		}
	}
	
	
	// end of file, add last set of cars to list.
	if (!cars.empty()) {
		toAdd.c.push_back(cars);
		toAdd.d.push_back(delays);
	}
	// Else nothing to add to location, so remove location from list of things to add.
	else { toAdd.lNo.pop_back(); }


	// Put Variables In Inputted Vector Pointers. //
	// Put cars into c vector.
	for (unsigned int i = 0; i < toAdd.c.size(); i++) {
		for (unsigned int j = 0; j < toAdd.c[i].size(); j++) {
			c->push_back(toAdd.c[i][j]);
		}
	}
	// spawn cars at locations from c vector.
	unsigned int cCount = 0;
	for (unsigned int i = 0; i < toAdd.c.size(); i++) {
		for (unsigned int j = 0; j < toAdd.c[i].size(); j++) {
			if ((!toAdd.d[i].empty()) && (toAdd.d[i][0] <= j)) {
				// If delay before car being checked, delete delay and add NULL to location spawn.
				// The location object understands to treat this as a delay. Also return to original 
				// j position to correctly keep adding cars.
				l->at(toAdd.lNo[i]).spawn(NULL);
				toAdd.d[i].erase(toAdd.d[i].begin());
				j--;
			}
			else { l->at(toAdd.lNo[i]).spawn(&c->at(cCount++)); }
		}
	}
}

// General function that can be used to determine how many systems there are in a design files.
unsigned int getRequiredOutputs(std::string fileName){
	// Open File 
	std::ifstream file(fileName);
	if (file.fail())
	{
		std::cout << "ERROR: can't find sim file, " << fileName << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		exit(EXIT_FAILURE);
	}
	
	// Load file into vector of lines.
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)){ lines.push_back(line); }

	
	// Parse file for systems.
	unsigned int systemCount = 0;
	for (unsigned int i = 0; i < lines.size(); i++) {
		if (lines[i] == "System") {
			systemCount++;
		}

		// As soon as road declarations are hit break. As system keyword in road will break 
		// function.
		else if (lines[i] == "Road") {
			break;
		}
	}
	return systemCount;
}

// -----------------------------------------------------------------------------------------------//
