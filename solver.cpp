#include "grid_fns.h"
#include "utils.h"
#include "advect.h"
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[]){
/* Below is basic skeleton of a fluid solver. Each function will be implemented,
and combine to give us a simulator.*/
	int numFrames = 60;
	int xDim = 32;
	int yDim = 32;

	float deltaT, t;
	int initValue = 1;

	// Use user inputs if given
	if (argc >= 4) {
		numFrames = atoi(argv[1]);
		xDim = atoi(argv[2]);
		yDim = atoi(argv[3]);
	}
	// 1. Initialize grids with fluid
	vector< vector<float> > pressureGrid(xDim, vector<float>(yDim, initValue));
	vector< vector<float> > horizVelocityGrid(xDim+1, vector<float>(yDim, initValue));
	vector< vector<float> > vertVelocityGrid(xDim, vector<float>(yDim+1, initValue));

	// Used for updating fields ins advect function
	vector< vector<float> > updatedHorizGrid(xDim+1, vector<float>(yDim, initValue));
	vector< vector<float> > updatedVertGrid(xDim, vector<float>(yDim+1, initValue));

	/*
	Used for creating timestep, eventually, don't need for right now

	vector<float>* bottomLeftVel = rightSideVel(horizVelocityGrid, vertVelocityGrid, 0, 0);
	float maxVel = magnitude(*bottomLeftVel);
	delete bottomLeftVel;
	*/

	const float TIME_PER_FRAME = 1 / 15.0;
	for (int i = 0; i < numFrames; ++i) {
		t = 0;
		while (t < TIME_PER_FRAME) {
			deltaT = 1 / maxVel;

			advect(horizVelocityGrid, vertVelocityGrid, updatedHorizGrid, updatedVertGrid, xDim, yDim, deltaT);
	 		// pressure Projection
	 		// advect free surface
			t = t + deltaT;
		}
	// 	save frame i
	}

}
