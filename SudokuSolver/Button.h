#pragma once
#include <stdlib.h>
#include "includes.h"

class Button
{
public:
	Button();
	~Button();
	enum State{ up, down };
	void Init(std::string i1, std::string i2, int ax, int ay,std::string awn);
	void Drawup(cv::Mat& dest);
	void Drawdown(cv::Mat& dest);
	bool handleEvent(cv::Mat& dest,int event, int mx, int my);
private:
	int x, y,xmax,ymax;
	int xl, yl;
	int ch;
	State state;
	std::string wn;
	cv::Mat Imgup, Imgdown;
};

