#include "stdafx.h"
#include "SudokuProblem.h"


void main()
{
	
	int initial[81] = {	0,0,0, 0,0,3, 2,9,0,
						0,8,6, 5,0,0, 0,0,0,
						0,2,0, 0,0,1, 0,0,0,

						0,0,3, 7,0,5, 1,0,0,
						9,0,0, 0,0,0, 0,0,8,
						0,0,2, 9,0,8, 3,0,0,

						0,0,0, 4,0,0, 0,8,0,
						0,0,0, 0,0,6, 4,7,0,
						0,4,7, 1,0,0, 0,0,0 };

	SudokuProblem sudokuProblem = SudokuProblem(initial);
	sudokuProblem.solve();
	waitKey(0);
}