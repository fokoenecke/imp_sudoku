#include "includes.h"
#include "SudokuProblem.h"
#include "SudokuSolver.h"

SudokuSolver sudoku;

void ButtonClick(int event, int x, int y, int flags, void* userdata)
{
	sudoku.ButtonClick(event, x, y, flags, userdata);
}

void main()
{
	setMouseCallback(sudoku.windowName, ButtonClick, NULL);
	sudoku.Run();
	
	waitKey(0);
}