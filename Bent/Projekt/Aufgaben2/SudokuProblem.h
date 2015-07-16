#pragma once
#include "stdafx.h"
class SudokuProblem
{
public:
	SudokuProblem(int initial[81]);
	~SudokuProblem();
	void print();
	Mat getImage();
	void solve();
private:
	vector<string> sudoku;
	vector<string> check;
	bool isSolved();
	

	void solvePreparation();
		bool finished[9][9];
		list<int> choices[9][9];
		void removeChoices();
		bool findSimpleSolutions();
		bool findRowSolutions(int row);
		bool findColSolutions(int col);
		bool findBlockSolutions(int block);
	
	bool solveRecursion(int row, int col);
	int recursionCounter = 0;
	bool checkRow(int row);
	bool checkCol(int col);
	bool checkBlock(int row, int col);
	struct Vector2
	{
		int x;
		int y;
	};
	
};

