#ifndef __GRIDFUNCTIONS__
#define __GRIDFUNCTIONS__

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;


/*
	inputFileName: string; name of input file to be read from
	Data format: Each row holds space seperated values for that row in toFill.
		row 0 data
		row 1 data
		...
		row n data
		---------------------------
		Ex:
		---------------------------
		1.2 1 3.4 3 23 2 ... 3 1
		3 9 9.3 8 23 0 ... 0 9
		...
		4 9.6 3 25 28 2.5 ... 3 6

	Return type: vector<float>*
*/
vector<float>* getInputData(string inputFileName) {
	/*
	Returns vector holding inputFileName's input data
	*/
	ifstream inputFile;
	string strValue;
	float fltValue;
	vector<float>* values = new vector<float>;

	inputFile.open(inputFileName, ios::in);
	while(inputFile >> strValue) {
		if (strValue != "\n") {
			fltValue = stof(strValue);
			values->push_back(fltValue);
		}
	}
	return values;
}

/*
	toFill: 2D vector of floats; holds default values, needs to be initialized
	inputFileName: string; file name of the input file

	Altered by reference: toFill
	Return type: void
*/
void fillGrid(vector< vector<float> > &toFill, string inputFileName) {
	/*
	Gets values from input file, fills toFill with them
	*/
	int numRows = toFill.size();
	int numCols = toFill.at(0).size();

	const vector<float>* inputData = getInputData(inputFileName);

	for(size_t i = 0; i < numRows; ++i) {
		for (size_t j = 0; j < numCols; ++j) {
			toFill.at(i).at(j) = inputData->at(i*numCols+j);
		}
	}

	delete inputData;
}

/*
	pressureGrid: 2D vector of floats; holds pressure values
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: float
*/
float correctPGet(const vector< vector<float> > &pressureGrid, int i, int j){
	/*
	Gets the (i,j)th value from the pressure grid.
	Used for consistency with horizontal and vertical Velocity getters.
	*/
	return pressureGrid.at(i).at(j);
}

/*
	horizVelocityGrid: 2D vector of floats; holds horizontal velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: float
*/
float correctHVGet(const vector< vector<float> > &horizVelocityGrid, int i, int j){
	/*
	Gets the correct half indice (i,j)th value from the horizVelocityGrid grid.
	Expects i to be a half index Ex: 4-.5
	Truncates index then adds 1; handles both i-.5 and i+.5
	*/
	return horizVelocityGrid.at(i+1).at(j);
}


/*
	verticalVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: float
*/
float correctVVGet(const vector< vector<float> > &verticalVelocityGrid, int i, int j){
	/*
	Gets the correct half indice (i,j)th value from the verticalVelocityGrid grid.
	Expects j to be a half index Ex: 4-.5
	Truncates index then adds 1; handles both j-.5 and j+.5
	*/
	return verticalVelocityGrid.at(i).at(j+1);
}

/*
	horizVelocityGrid: 2D vector of floats; holds horizontal velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: float
*/
float horCenterVel(const vector< vector<float> > &horizVelocityGrid, int i, int j) {
	/*
	Returns horizontal velocity at center of grid cell.
	Averages horizontal velocities at left and right sides of the cell.
	*/
	float numeratorTerm1 = correctHVGet(horizVelocityGrid, i-.5, j);
	float numeratorTerm2 = correctHVGet(horizVelocityGrid, i+.5, j);
	return (numeratorTerm1 + numeratorTerm2) / 2;
}


/*
	vertVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: float
*/
float verCenterVel(const vector< vector<float> > &vertVelocityGrid, int i, int j) {
	/*
	Returns vertical velocity at center of grid cell.
	Averages vertical velocities at top and bottom sides of the cell.
	*/
	float numeratorTerm1 = correctVVGet(vertVelocityGrid, i, j-.5);
	float numeratorTerm2 = correctVVGet(vertVelocityGrid, i, j+.5);
	return (numeratorTerm1 + numeratorTerm2) / 2;
}


/*
	horizVelocityGrid: 2D vector of floats; holds vertical horizontal components at 1/2 indices
	vertVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: vector<float>* of size 2
*/
vector<float>* centerVel(const vector< vector<float> > &horizVelocityGrid, const vector< vector<float> > &vertVelocityGrid, int i, int j) {
	/*
	Returns x and y components of velocity at center of the cell
	*/
	vector<float>* velocityComponents  = new vector<float>;
	velocityComponents->push_back(horCenterVel(horizVelocityGrid, i, j));
	velocityComponents->push_back(verCenterVel(vertVelocityGrid, i, j));
	return velocityComponents;
}


/*
	horizVelocityGrid: 2D vector of floats; holds vertical horizontal components at 1/2 indices
	vertVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: vector<float>* of size 2
*/
vector<float>* rightSideVel(const vector< vector<float> > &horizVelocityGrid, const vector< vector<float> > &vertVelocityGrid, int i, int j) {
	/*
	Returns x and y components of velocity at right side of the cell.
	Averages the vertical velocities at the top and bottoms of the cells to the left and right.
	*/
	float horizComponent = correctHVGet(horizVelocityGrid, i+.5, j);\

	float vertComponentNumerator1 = correctVVGet(vertVelocityGrid, i, j-.5) + correctVVGet(vertVelocityGrid, i, j+.5);
	float vertComponentNumerator2 = correctVVGet(vertVelocityGrid, i+1, j-.5) + correctVVGet(vertVelocityGrid, i+1, j+.5);
	float vertComponent = (vertComponentNumerator1 + vertComponentNumerator2) / 4;
	return new vector<float> {horizComponent, vertComponent};
}

/*
	horizVelocityGrid: 2D vector of floats; holds vertical horizontal components at 1/2 indices
	vertVelocityGrid: 2D vector of floats; holds vertical velocity components at 1/2 indices
	i: integer; index for ith row
	j: integer; index for jth column

	Return type: vector<float>* of size 2
*/
vector<float>* topSideVel(const vector< vector<float> > &horizVelocityGrid, const vector< vector<float> > &vertVelocityGrid, int i, int j) {
	/*
	Returns x and y components of velocity at top side of the cell.
	Averages the horizontal velocities at the left and right of the cells to the top and bottom.
	*/
	float horizComponentNumerator1 = correctHVGet(horizVelocityGrid, i-.5, j) + correctHVGet(horizVelocityGrid, i+.5, j);
	float horizComponentNumerator2 = correctHVGet(horizVelocityGrid, i-.5, j+1) + correctHVGet(horizVelocityGrid, i+.5, j+1);
	float horizComponent = (horizComponentNumerator1 + horizComponentNumerator2) / 4;

	float vertComponent = correctVVGet(vertVelocityGrid, i, j+.5);
	return new vector<float> {horizComponent, vertComponent};
}
#endif
