#ifndef __ADVECT__
#define __ADVECT__

#include <cmath>
using namespace std;


// for every grid cell:
// 	calculate negative central velocity
// 	grid cell coords -> X
// 	X_prev = X - neg central velocity * deltaT
// 	need to bound x_prev with array dims
// 	update current cell with velocity at X_prev

/*
	input: float; value to be bounded between 0 and maxSize
	maxSize: float; maximum bound for input

	Return type: float
*/
float bound(float input, float maxSize){
	// Bounds input between 0 and maxSize, inclusive
	float zero = 0;
	input = min(input, maxSize);
	input = max(input, zero);
	return input;
}

/*
	factor: float&; possibly incorrect interpolation factor
	cell_prev: int; cell index closest to prev
	prev: float; location in velocity field at previous time step

	Alters by reference: factor
	Return type: vector<float> of size 2
*/
vector<int> cellLeftOrRight(float &factor, int cell_prev, float prev, int max) {
	/*
	Returns the correct cell indices to use in interpolation.
	May change factor if prev is to the left/below cell_prev.
	*/
	int cell_left = 0;
	int cell_right = 0;

	if (factor <= 0) {
		cell_left = cell_prev - 1;
		cell_right = cell_prev;
		factor = prev - cell_left;
	}
	else {
		cell_left = cell_prev;
		cell_right = cell_prev + 1;
	}
	return vector<int> {bound(cell_left, max), bound(cell_right, max)};
}

/*
	horizVelocityGrid: 2D vector of floats; holds vertical horizontal components at 1/2 indices
	vertVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	updatedHorizGrid: 2D vector of floats; will hold the updated horizontal velocities
	updatedVertGrid: 2D vector of floats; will hold the updated vertical velocities
	xDim: int; number of cells in x direction
	yDim: int; number of cells in y direction
	deltaT: float; time step over which we advect the velocity
	i: integer; index for ith row
	j: integer; index for jth column

	Alters by reference: updatedHorizGrid and updatedVertGrid
	Return type: void
*/
void advect(vector< vector<float> > &horizVelocityGrid, vector< vector<float> > &vertVelocityGrid, vector< vector<float> >& updatedHorizGrid, vector< vector<float> >& updatedVertGrid, int xDim, int yDim, float deltaT) {
	/*
	Updates the velocity field due to advection.
	*/

	// For all grid cells
	for (size_t i = 0; i < xDim; ++i) {
		for (size_t j = 0; j < yDim; ++j) {
			// Get center velocity for (i,j)th cell
			vector<float>* centerVelocity_n1 = centerVel(horizVelocityGrid, vertVelocityGrid, i, j);
			// Trace velocity at (i,j) backwards over timeframe deltaT
			float x_prev = bound(i - centerVelocity_n1->at(0) * deltaT, xDim-1);
			float y_prev = bound(j - centerVelocity_n1->at(1) * deltaT, yDim-1);

			// Get cell of floating point locations
			int cell_x_prev = round(x_prev);
			int cell_y_prev = round(y_prev);

			// Interpolate floating point location with velocities at cell locations
			float alpha = x_prev - cell_x_prev;
			float beta = y_prev - cell_y_prev;
			vector<int> xCellsLeftAndRight = cellLeftOrRight(alpha, cell_x_prev, x_prev, xDim);
			vector<int> yCellsTopAndBottom = cellLeftOrRight(alpha, cell_y_prev, y_prev, yDim);
			float horizVelTerm1 = (1-alpha) * horizVelocityGrid.at(xCellsLeftAndRight.at(0)).at(cell_y_prev);
			float horizVelTerm2 = alpha * horizVelocityGrid.at(xCellsLeftAndRight.at(1)).at(cell_y_prev);

			// Update horizontal velocity of current cell with interpolated velocity at previous location
			updatedHorizGrid.at(i).at(j) = horizVelTerm1 + horizVelTerm2;
			float vertVelTerm1 = (1-alpha) * vertVelocityGrid.at(cell_x_prev).at(yCellsTopAndBottom.at(0));
			float vertVelTerm2 = alpha * vertVelocityGrid.at(cell_x_prev).at(yCellsTopAndBottom.at(1));
			// Update vertical velocity of current cell with interpolated velocity at previous location
			updatedVertGrid.at(i).at(j) = vertVelTerm1 + vertVelTerm2;

			delete centerVelocity_n1;
		}
	}
}

#endif
