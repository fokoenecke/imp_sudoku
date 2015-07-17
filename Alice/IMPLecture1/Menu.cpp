#include "includes.h"
#include "Menu.h"


Menu::Menu()
{
	std::cout << "Aufruf: Menu Konstruktor" << std::endl;
	bar_name = "menu";
}
Menu::~Menu()
{
	std::cout << "Menu Destruktor wird aufgerufen" << std::endl;

}
void Menu::Draw(SudokuSolver& sudokusolver)
{
	cv::imshow(mainwindow, imgbackground);
	addButtons(sudokusolver);
}
void Menu::addButtons(SudokuSolver& sudokusolver)
{
	
	//cv::createButton(bar_name, sudokusolver.getSolution(-1), NULL, CV_PUSH_BUTTON);
	//cv::createButton(bar_name, sudokusolver.progExit(-1), NULL, CV_PUSH_BUTTON);

}