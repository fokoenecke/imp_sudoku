#ifndef SUDOKUSOLVER_HPP
#define SUDOKUSOLVER_HPP
#pragma once
#include "includes.h"
#include "Button.h"


//#include "Menu.h"

class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	enum PState{psExit,psCapture,psWait,psFind,psSolution,psAgain};
	// draw functions
	void drawSolution();
	inline void drawBackground(cv::Mat& img);
	// callback functions
	void getImage();
	void getAgain();
	void getSolution(cv::Mat src);
	void showSolution();
	void progExit();
	void Run();
	inline void showVideo();
	bool fertig = false;
	const char* windowName = "Sudoku Solver";
	const char* videoName = "Video";
	const char* capName = "Capture";
	const char* solName = "Solution";
	void ButtonClick(int event, int x, int y, int flags, void* userdata);
	
	//Menu menu;
	PState pstate;
	
	cv::Mat vidImage,rawImage;
	cv::Mat resultImage;
	cv::Mat screen;
	cv::Mat bg_Menu;
	Button b_capture, b_again, b_find, b_exit;
	int xmin, ymin, xmax, ymax;
	int x, y, xlen, ylen;
	int xchannel;
	cv::VideoCapture cap;
private:
	
};
#endif
