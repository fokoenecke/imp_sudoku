#include "stdafx.h"
#include "SudokuProblem.h"


void main()
{
	
	int initial[81] = {	0,0,0, 0,0,0, 7,0,8,
						0,0,8, 0,0,0, 0,0,0,
						5,0,0, 0,0,0, 2,0,0,

						0,0,0, 0,0,8, 0,6,0,
						9,0,0, 0,3,0, 0,0,7,
						0,3,0, 6,0,0, 0,0,0,

						0,0,9, 0,0,0, 0,0,5,
						0,0,0, 0,0,0, 4,0,0,
						2,0,1, 0,0,0, 0,0,0 };

	SudokuProblem sudokuProblem = SudokuProblem(initial);
	sudokuProblem.solve();
	sudokuProblem.getImage();
	waitKey(0);
}