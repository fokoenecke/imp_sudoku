#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <memory>
#include <thread>

using namespace std;
using namespace cv;

void Split(Mat* src, Mat* dest, int xd, int  yd, int xl, int yl, int channel);

bool InRange(int x, int y, int x1, int y1, int x2, int y2);
