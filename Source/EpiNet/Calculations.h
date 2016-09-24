// Author: Jacob Busfield
// Date: 08/05/2016
#pragma once
#include <stdlib.h>     
#include <time.h>      
#include <vector>
#include <algorithm>

// Random double between specified range. If no range is given then between 0 and 1.
double randDouble(double lower = 0, double upper = 1);

// Random int between specified range. If no range is given then between 0 and 10.
int randInt(int lower = 0, int upper = 10);

// Return median of all values in vector v.
double getMedian(std::vector<double> v);

// Return mean of all values in vector v.
double getMean(std::vector<double> v);

