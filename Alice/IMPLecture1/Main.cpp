#include "SudokuSolver.h"
#include "includes.h"

using namespace cv;

SudokuSolver sudoku;

void ButtonClick(int event, int x, int y, int flags, void* userdata)
{
	sudoku.ButtonClick(event, x, y, flags, userdata);
}

int main(int argc, char* argv[])
{
	setMouseCallback(sudoku.windowName, ButtonClick, NULL);
	sudoku.Run();
}