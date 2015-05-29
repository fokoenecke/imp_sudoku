#include "stdafx.h"
#include "SudokuProblem.h"

using namespace std;

int* solveSudokuArray(int initial[])
{
	
	return initial;
}

void main()
{
	int initial[81] = {	0,0,0, 0,0,0, 7,3,0,
						5,0,0, 0,0,0, 0,1,0,
						0,0,1, 0,0,6, 0,0,0,

						7,0,0, 0,0,2, 0,0,0,
						0,0,0, 6,0,8, 0,0,0,
						0,0,0, 5,0,0, 0,0,8,

						0,0,0, 2,0,0, 6,0,0,
						0,0,0, 0,0,0, 0,0,9,
						0,0,0, 0,0,0, 0,0,0 };

	SudokuProblem sudokuProblem = SudokuProblem(initial);
	sudokuProblem.print();
	sudokuProblem.solve();
	sudokuProblem.print();

	waitKey(0);
}