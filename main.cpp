#include <opencv2/core/core.hpp>
#include "Sudoku.h"

using namespace cv;

int main(int argc, char *argv[])
{
    Mat img = imread("img/sudoku_002.png", CV_LOAD_IMAGE_COLOR);
    if(img.empty())
       return -1;

    Sudoku sudoku = Sudoku(img);
    sudoku.detect();

    return 0;
}



