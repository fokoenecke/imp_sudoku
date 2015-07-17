#include "includes.h"
#include<thread>
#include "SudokuSolver.h"

using namespace cv;

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
cv::Mat SudokuSolver::findSolution(cv::Mat src)
{    // hier sollte der Lösungsalgorithmus stehen
	cv::Mat grey,edge;
	cv::cvtColor(src, grey, cv::COLOR_BGR2GRAY);
	blur(grey, edge, Size(3, 3));
	Canny(edge, edge, 8, 50);
	return edge.clone();
}
void SudokuSolver::getSolution(cv::Mat src)
{
	// erstelle die Lösung auf src und gib sol zurück
	resultImage = findSolution(src);
	if (!resultImage.empty()) { pstate = psSolution; }
}
void SudokuSolver::progExit()
{
	destroyAllWindows();
	fertig = true;
}
void SudokuSolver::showSolution()
{
	destroyWindow(capName);
	namedWindow(solName, 1);
	imshow(solName, resultImage);
	pstate = psWait;
}
void SudokuSolver::Run()
{
	fertig = false; pstate = psWait;
	while (!fertig)
	{
		showVideo();
		char key = (char)waitKey(10);
		switch(key)
		{
			case 'x':{pstate = psExit; break; } // exit
			case 'c':{pstate = psCapture; break; }
			case 'a':{pstate = psAgain; break; } // again
			case 's':{pstate = psFind; break;}
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
