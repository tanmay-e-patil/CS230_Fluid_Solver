#ifndef __UTILS__
#define __UTILS__

#include "grid_fns.h"
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

/*
	Return type: void
*/
void clearOutputFile(string fileName, int numFrames) {
	/*
	Opens and clears the given fileName for later use.
	Saves number of frames for use in Unity.
	*/
	ofstream outputFile;
	outputFile.open(fileName, ios::out | ios::trunc);
	outputFile << numFrames << endl;
	outputFile.close();
}

/*
	vec: vector of floats by reference; the vector to be output

	Return type: string
*/
string outputVector(vector<float> &vec) {
	/*
	Returns vec's values separated by spaces.
	*/
	stringstream s;
	for (size_t i = 0; i < vec.size()-1; ++i) {
		s << vec.at(i) << " ";
	}
	s << vec.at(vec.size()-1);
	return s.str();
}

/*
	horizVelocityGrid: 2D vector of floats; holds vertical horizontal components at 1/2 indices
	vertVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	xDim: int; number of cells in x direction
	yDim: int; number of cells in y direction
	fileName: string; the file to output to

	Return type: void
*/
void saveVelocityField(vector< vector<float> > &horizVelocityGrid, vector< vector<float> > &vertVelocityGrid, int xDim, int yDim, string fileName) {
	/*
	Saves center velocities of given horizontal and vertical velocity fields to fileName.
	*/
	vector<float>* centerVelocity;
	ofstream outputFile;
	outputFile.open(fileName, ios::app);
	outputFile << "Start Matrix" << endl;
	for (size_t i = 0; i < xDim; ++i) {
		for (size_t j = 0; j < yDim-1; ++j) {
			centerVelocity = centerVel(horizVelocityGrid, vertVelocityGrid, i, j);
			outputFile << outputVector(*centerVelocity);
			outputFile << ";";
			delete centerVelocity;
		}
		centerVelocity = centerVel(horizVelocityGrid, vertVelocityGrid, i, yDim-1);
		outputFile << outputVector(*centerVelocity);
		delete centerVelocity;
		outputFile << endl;
	}
	outputFile << "End Matrix" << endl;
	outputFile.close();
}


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
