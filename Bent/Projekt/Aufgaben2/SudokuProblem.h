#pragma once
#include "stdafx.h"
using namespace std;
class SudokuProblem
{
public:
	SudokuProblem(int initial[81]);
	~SudokuProblem();
	void print();
	int* solve();
	int working[81];
protected:

};

