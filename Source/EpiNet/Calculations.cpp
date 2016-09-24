// Author: Jacob Busfield
// Date: 08/05/2016
#include "stdafx.h"
#include "Calculations.h"

// Private, static variable used to initialise random number generator on first function call.
static bool initFlag = false;

// Random double between specified range. If no range is given then between 0 and 1.
double randDouble(double lower /* = 0 */, double upper /* = 1 */){
	/* Initialize random seed ONCE */
	if (!initFlag) {
		srand((unsigned int)time(NULL));
		initFlag = true;
	}
	return lower + static_cast <double> (rand()) / (static_cast <float> (RAND_MAX / (upper - lower)));
}

// Random int between specified range. If no range is given then between 0 and 10.
int randInt(int lower /* = 0 */, int upper /* = 10 */){
	/* Initialize random seed ONCE */
	if (!initFlag) {
		srand((unsigned int)time(NULL));
		initFlag = true;
	}
	if ((upper - lower) > 0) { // Protect against mod(0)
		return lower + (rand() % (upper - lower));
	}
	return lower;
}

// Return median of all values in vector v.
double getMedian(std::vector<double> v){
	if (v.empty()) { return 0; }
	else {
		std::sort(v.begin(), v.end());
		if (v.size() % 2 == 0){ return (v[v.size() / 2 - 1] + v[v.size() / 2]) / 2; }
		else{ return v[v.size() / 2]; }
	}
}

// Return mean of all values in vector v.
double getMean(std::vector<double> v){
	if (v.empty()) { return 0; }
	else {
		double temp = 0;
		for (unsigned int i = 0; i < v.size(); i++) {
			temp += v[i];
		}
		temp /= v.size();
		return temp;
	}
}

