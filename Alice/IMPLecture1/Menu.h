#ifndef MENU_HPP
#define MENU_HPP
#pragma once
#include "includes.h"
#include "SudokuSolver.h"


class Menu
{
public:
	Menu();
	~Menu();
	void addButtons(SudokuSolver & sudokusolver);
	void Draw(SudokuSolver & sudokusolver);

	const char* bar_name;
	const char* mainwindow;
	cv::Mat imgbackground;
};
#endif
