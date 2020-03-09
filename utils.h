#ifndef __UTILS__
#define __UTILS__

#include <cmath>
using namespace std;


/*
input: vector of floats; vector for which to calc magnitude

Return type: float
*/
float magnitude(vector<float> &input){
	// Returns magnitude of input
	float mag = 0;
	for (size_t i = 0; i < input.size(); ++i) {
		mag += pow(input.at(i), 2);
	}
	return pow(mag, .5);
}

#endif
