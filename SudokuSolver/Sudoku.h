#ifndef SUDOKU_H
#define SUDOKU_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;

class Sudoku {
    public:
        Sudoku(Mat&);
        virtual ~Sudoku();
        void getOneDimSudoku(int initial[]);

        bool detect();
    protected:
    private:
        Mat sudokuImage;
        Mat warped;
        Mat debugImage;

        int digits[9][9];
        vector<Vec3f> horizontalLines;
        vector<Vec3f> verticalLines;

        void drawRects(Mat& image, vector<Rect>&);
        void drawLines(Mat& image, vector<Vec2f>&);
        void separateLines(vector<Vec2f>&, float tolerance);
        void extractSudoku(Mat& image);

        vector<Rect> findRects();
        void recognizeDigits(vector<Rect>&);
        float roundToPlaces(float, int);

        void printDigits();
        void checkUserFeedback();
};

#endif // SUDOKU_H
