#include "Sudoku.h"

using namespace std;

struct myclass {
    bool operator() (Vec2f vec1, Vec2f vec2) { return (vec1[0] < vec2[0]);}
} myobject;

Sudoku::Sudoku(Mat& image) {
    sudokuImage = image;
}

bool Sudoku::detect() {

    Mat sudokuGrayscaled, canny, cannyWithLines;

    namedWindow("sudoku", CV_WINDOW_AUTOSIZE );

	cvtColor(sudokuImage, sudokuGrayscaled, CV_RGB2GRAY);
	blur(sudokuGrayscaled, sudokuGrayscaled, Size(3,3));

    Canny(sudokuGrayscaled, canny, 50, 200);
    cvtColor(canny, cannyWithLines, CV_GRAY2BGR);

    vector<Vec2f> lines;
	HoughLines(canny, lines, 1, CV_PI/180, 120);
    //drawLines(lines);
    vector<Rect> rects = findRects(lines);
    for( size_t i = 0; i < rects.size(); i++ ) {
        rectangle(sudokuImage, rects[i], Scalar(0,255,0));
        cout << rects[i] << endl;
    }

    imshow("sudoku", sudokuImage);

    waitKey();
    return false;
}

vector<Rect> Sudoku::findRects(vector<Vec2f>& lines) {

    float vertivalTheta = 0;
    float horizontalTheta = (CV_PI/2);

    vector<Vec2f> horizontalLines;
    vector<Vec2f> verticalLines;

    vector<Vec2f> cleanedHLines;
    vector<Vec2f> cleanedVLines;

	for( size_t i = 0; i < lines.size(); i++ ) {

        float theta = lines[i][1];
        if (theta == horizontalTheta) {
            horizontalLines.push_back(lines[i]);
        } else if (theta == vertivalTheta) {
            verticalLines.push_back(lines[i]);
        }

    }

    sort(horizontalLines.begin(), horizontalLines.end(), myobject);
    sort(verticalLines.begin(), verticalLines.end(), myobject);

    int sudokuColumnWidthThreshold = sudokuImage.cols / (9*2);
    int lastRho = -sudokuColumnWidthThreshold;
    for( size_t i = 0; i < horizontalLines.size(); i++ ) {
        if ((horizontalLines[i][0] - lastRho) > sudokuColumnWidthThreshold) {
            cleanedHLines.push_back(horizontalLines[i]);
        } else {
             cleanedHLines.back()[0] = horizontalLines[i][0];
        }
        lastRho = horizontalLines[i][0];
    }
    cout << cleanedHLines.size() << endl;

    int sudokuRowHeightThreshold = sudokuImage.rows / (9*2);
    lastRho = -sudokuRowHeightThreshold;
    for( size_t i = 0; i < verticalLines.size(); i++ ) {
        if ((verticalLines[i][0] - lastRho) > sudokuRowHeightThreshold) {
            cleanedVLines.push_back(verticalLines[i]);
        } else {
            cleanedVLines.back()[0] = verticalLines[i][0];
        }
        lastRho = verticalLines[i][0];
    }
    cout << cleanedVLines.size() << endl;


    vector<Rect> rects;
    for( size_t i = 0; i < cleanedHLines.size() -1; i++ ) {
        for( size_t j = 0; j < cleanedVLines.size() -1; j++ ) {
            rects.push_back(Rect(cleanedVLines[j][0], cleanedHLines[i][0], cleanedVLines[j+1][0] - cleanedVLines[j][0], cleanedHLines[i+1][0] - cleanedHLines[i][0]));
        }
    }

    return rects;
}

void Sudoku::drawLines(vector<Vec2f>& lines) {

	for( size_t i = 0; i < lines.size(); i++ ) {

        float rho = lines[i][0];
        float theta = lines[i][1];

        double a = cos(theta);
		double b = sin(theta);

        double x0 = a*rho;
		double y0 = b*rho;

        Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)), cvRound(y0 - 1000*(a)));

        line(sudokuImage, pt1, pt2, Scalar(0,0,255), 1, 8);
    }

}

float Sudoku::roundToPlaces(float val, int places) {
    int pot = pow(10, places);
    return floorf(val * pot) / pot;
}

Sudoku::~Sudoku() {
    //dtor
}
