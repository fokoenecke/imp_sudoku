#ifndef SUDOKU_H
#define SUDOKU_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <math.h>
#include <algorithm>

using namespace cv;

class Sudoku {
    public:
        Sudoku(Mat&);
        virtual ~Sudoku();

        bool detect();
    protected:
    private:
        Mat sudokuImage;
        void drawLines(vector<Vec2f>&);
        vector<Rect> findRects(vector<Vec2f>&);
        float roundToPlaces(float, int);
};

#endif // SUDOKU_H
