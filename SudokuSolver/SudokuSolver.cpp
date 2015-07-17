#include "includes.h"
#include<thread>
#include "SudokuSolver.h"
#include "SudokuProblem.h"

using namespace cv;

void SudokuSolver::Run()
{
	fertig = false; pstate = psWait;
	while (!fertig)
	{
		showVideo();
		char key = (char)waitKey(10);
		switch (key)
		{
		case 'x':{pstate = psExit; break; } // exit
		case 'c':{pstate = psCapture; break; }
		case 'a':{pstate = psAgain; break; } // again
		case 's':{pstate = psFind; break; }
		}

		switch (pstate)
		{
		case psExit:{progExit(); return; }
		case psCapture:{getImage(); break; }
		case psFind:{getSolution(rawImage); break; }
		case psAgain:{getAgain(); break; }
		case psSolution:{showSolution(); break; }
		}
	}
}

void SudokuSolver::getSolution(cv::Mat src)
{
	// Umwandlung des Bildes in ein Array für SudokuProblem

	// ... Oles Part

	// Lösungsalgorithmus
	int initial[81] = { 
		0, 0, 0,	0, 0, 0,	7, 0, 8,
		0, 0, 8,	0, 0, 0,	0, 0, 0,
		5, 0, 0,	0, 0, 0,	2, 0, 0,

		0, 0, 0,	0, 0, 8,	0, 6, 0,
		9, 0, 0,	0, 3, 0,	0, 0, 7,
		0, 3, 0,	6, 0, 0,	0, 0, 0,

		0, 0, 9,	0, 0, 0,	0, 0, 5,
		0, 0, 0,	0, 0, 0,	4, 0, 0,
		2, 0, 1,	0, 0, 0,	0, 0, 0 };

	SudokuProblem sudokuProblem = SudokuProblem(initial);
	sudokuProblem.solve();
	resultImage = sudokuProblem.getImage();

	if (!resultImage.empty()) { pstate = psSolution; }
}

void SudokuSolver::showSolution()
{
	destroyWindow(capName);
	namedWindow(solName, 1);
	imshow(solName, resultImage);
	pstate = psWait;
}


SudokuSolver::SudokuSolver()
{
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	//(menu.mainwindow = windowName;
	
	bg_Menu = cv::imread("images/interface.png");
	b_capture.Init("images/b_capture_up.png", "images/b_capture_down.png", 880, 280,windowName);
	b_again.Init("images/b_again_up.png", "images/b_again_down.png", 880, 385, windowName);
	b_find.Init("images/b_solution_up.png", "images/b_solution_down.png", 880, 490, windowName);
	b_exit.Init("images/b_exit_up.png", "images/b_exit_down.png", 880, 595, windowName);

	b_capture.Drawup(bg_Menu);
	b_again.Drawup(bg_Menu);
	b_find.Drawup(bg_Menu);
	b_exit.Drawup(bg_Menu);

	screen = bg_Menu.clone();
	drawBackground(screen);
	//namedWindow(videoName, 1);
	cap.open(0);
	if (!cap.isOpened()){}// fehler ausgeben
	//std::thread t(showVideo, windowName, cap, screen, 50, 50);
	//t.join();
	std::cout << "Aufruf: Konstruktor SudokuSolver" << std::endl;
}
void SudokuSolver::ButtonClick(int event, int x, int y, int flags, void* userdata)
{
	if (b_capture.handleEvent(screen, event, x, y))
	{
		//drawBackground(screen);
		pstate = psCapture;
	}
	else
	if (b_again.handleEvent(screen, event, x, y))
	{
		//exit button
		pstate = psAgain;// progExit();
	}
	else
	if (b_find.handleEvent(screen, event, x, y))
	{
		//exit button
		pstate = psFind;// progExit();
	}
	else
	if (b_exit.handleEvent(screen, event, x, y))
	{
		//exit button
		pstate = psExit;// progExit();
	}
	
}

// Callback_Funktionen
void SudokuSolver::getImage()
{
	cap >> rawImage;
	namedWindow(capName, CV_WINDOW_AUTOSIZE);
	imshow(capName, rawImage);
	pstate = psWait;
}

void SudokuSolver::getAgain()
{
	destroyWindow(capName);
	rawImage.release();
	destroyWindow(solName);
	resultImage.release();
	getImage();
}


void SudokuSolver::progExit()
{
	destroyAllWindows();
	fertig = true;
}

SudokuSolver::~SudokuSolver()
{
	std::cout << "Aufruf: Destruktor SudokuSolver" << std::endl;
	destroyWindow(windowName);
}
inline void SudokuSolver::drawBackground(cv::Mat& img){imshow(windowName, img);}

inline void SudokuSolver::showVideo()
{
	cap >> vidImage;
	Split(&vidImage, &screen, 79, 253, vidImage.size().width,
		vidImage.size().height,
		vidImage.channels());
	imshow(windowName, screen);
}
