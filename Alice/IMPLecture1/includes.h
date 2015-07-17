#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <opencv2\opencv.hpp>

void Split(cv::Mat* src, cv::Mat* dest,
	int xd, int  yd, int xl, int yl, int channel);

bool InRange(int x, int y, int x1, int y1, int x2, int y2);

//void showVideo(std::string wn, cv::VideoCapture caption, cv::Mat dest, int x, int y);