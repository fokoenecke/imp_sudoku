#include "Sudoku.h"
#include "Helpers.h"

struct DistSorter {
    bool operator() (Vec3f vec1, Vec3f vec2) { return (vec1[2] < vec2[2]);}
} distsorter;

const int NO_OF_BOXES = 9;
Sudoku::Sudoku(Mat& image) {
    sudokuImage = image;
    // init digit array with zeroes
    for (int row = 0; row < NO_OF_BOXES; row++) {
        for (int col = 0; col < NO_OF_BOXES; col++) {
            digits[row][col] = 0;
        }
    }
}

int* Sudoku::getOneDimSudoku() {
    int oneDimSudoku[NO_OF_BOXES*NO_OF_BOXES];
    for (int row = 0; row < NO_OF_BOXES; ++row) {
        for (int col = 0; col < NO_OF_BOXES; ++col) {
           oneDimSudoku[(row*NO_OF_BOXES)+col] = digits[row][col];
        }
    }
    return oneDimSudoku;
}

bool Sudoku::detect() {

    // prepare the initial image for edge detection
    Mat sudokuGrayscaled, canny;
	cvtColor(sudokuImage, sudokuGrayscaled, CV_RGB2GRAY);
	blur(sudokuGrayscaled, sudokuGrayscaled, Size(3,3));
    // find edges
    Canny(sudokuGrayscaled, canny, 50, 200);
    // find lines in the original image
    vector<Vec2f> lines;
	HoughLines(canny, lines, 0.75, CV_PI/180, 110);
	// sort lines by horizontal and vertical orientation (and sort them by x/y)
    separateLines(lines, 0.05);

    // find corners, crop image and change its perspective
    extractSudoku(sudokuImage);
    debugImage = warped.clone();
    // prepare the warped image for edge detection
	cvtColor(warped, sudokuGrayscaled, CV_RGB2GRAY);
	blur(sudokuGrayscaled, sudokuGrayscaled, Size(3,3));
    // find edges
    Canny(sudokuGrayscaled, canny, 50, 200);
    // find edges
	HoughLines(canny, lines, 0.75, CV_PI/180, 110);

    // prepare the lines of our new image, the lines should be more acurate now
    separateLines(lines, 0.02);
    drawLines(debugImage, lines);
    vector<Rect> rects = findRects();

    // only go on, if the right number of boxes were found
    if (rects.size() == NO_OF_BOXES*NO_OF_BOXES) {
        drawRects(debugImage, rects);
        recognizeDigits(rects);
        checkUserFeedback();
    }

    //imshow("debug", debugImage);
    return false;
}

void Sudoku::separateLines(vector<Vec2f>& lines, float tolerance) {

    verticalLines.clear();
    horizontalLines.clear();
	for( size_t i = 0; i < lines.size(); i++ ) {
        // calculate intersection with axes
        float theta = lines[i][1];
        float rho = lines[i][0];

        double a = cos(theta);
		double b = sin(theta);

        double x0 = a*rho;
		double y0 = b*rho;

        Vec3f line;
        line[0] = rho;
        line[1] = theta;

        // check if our line is nearly horizontal or vertical
        // theta around 0 means vertical
        if (theta > CV_PI-tolerance || theta < tolerance) {
            line[2] = x0;
            verticalLines.push_back(line);
        // theta around PI/2 means horizontal
        } else if (theta > (CV_PI/2)-tolerance && theta < (CV_PI/2)+tolerance) {
            line[2] = y0;
            horizontalLines.push_back(line);
        }
    }

        // sort both line arrays by deistance from origin
    sort(verticalLines.begin(), verticalLines.end(), DistSorter());
    sort(horizontalLines.begin(), horizontalLines.end(), DistSorter());

}

void Sudoku::extractSudoku(Mat& image) {

    // get the first and the last of our lines, to check the corner points
    Vec3f hFirst, hLast, vFirst, vLast;
    hFirst = horizontalLines.front();
    hLast = horizontalLines.back();
    vFirst = verticalLines.front();
    vLast = verticalLines.back();

	vector<cv::Point2f> sortedCorners;
    // compute intersections and give them some offset
    Point2f p1 = Helpers::computeIntersectionOfTwoLines(Vec2f(hFirst[0], hFirst[1]), Vec2f(vFirst[0], vFirst[1]));
    p1.x = std::max(p1.x - 2, .0f);
    p1.y = std::max(p1.y - 2, .0f);
    sortedCorners.push_back(p1);

    Point2f p2 = Helpers::computeIntersectionOfTwoLines(Vec2f(hFirst[0], hFirst[1]), Vec2f(vLast[0], vLast[1]));
    p2.x = std::min(p2.x + 2, (float)sudokuImage.cols);
    p2.y = std::max(p2.y - 2, .0f);
    sortedCorners.push_back(p2);

    Point2f p3 = Helpers::computeIntersectionOfTwoLines(Vec2f(hLast[0], hLast[1]), Vec2f(vLast[0], vLast[1]));
    p3.x = std::min(p3.x + 2, (float)sudokuImage.cols);
    p3.y = std::min(p3.y + 2, (float)sudokuImage.rows);
    sortedCorners.push_back(p3);

    Point2f p4 = Helpers::computeIntersectionOfTwoLines(Vec2f(hLast[0], hLast[1]), Vec2f(vFirst[0], vFirst[1]));
    p4.x = std::max(p4.x - 2, .0f);
    p4.y = std::min(p4.y + 2, (float)sudokuImage.rows);
    sortedCorners.push_back(p4);

    //for (int i=0; i<sortedCorners.size(); i++) {
    //    cout << sortedCorners[i] << endl;
    //}

    // Viereck erstellen
	Mat quad = Mat::zeros(400, 400, CV_8UC1);

	// Eckpunkte des Vierecks definieren
	std::vector<cv::Point2f> quad_pts;
	quad_pts.push_back(cv::Point2f(0, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, 0));
	quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
	quad_pts.push_back(cv::Point2f(0, quad.rows));

	// Transformations-Matrix berechnen lassen
	Mat transmtx = getPerspectiveTransform(sortedCorners, quad_pts);
	// Perspektivische Projektion anwenden
	warpPerspective(image, quad, transmtx, quad.size());
	warped = quad;

}

vector<Rect> Sudoku::findRects() {

    // find a threshold to distinguish doubled lines based on image width
    // the gap between two lines should be at least 3/4 an estimated digit box
    int distanceThreshold = warped.rows / (NO_OF_BOXES*1.5);
    int lastDistance = distanceThreshold;
    int boxCounter = 0;
    Vec2f boxXValues[NO_OF_BOXES] = {0};
    for( size_t i = 0; i < verticalLines.size(); i++ ) {
        if ((verticalLines[i][2] - lastDistance) > distanceThreshold) {
            // a big gab always marks the end of the old box and a potential beginning of a new box
            boxXValues[boxCounter][1] = verticalLines[i][2];
            boxCounter++;
            if (boxCounter == 9) break;
            boxXValues[boxCounter][0] = verticalLines[i][2];
        } else {
            // small gaps only affect the start of a box
            boxXValues[boxCounter][0] = verticalLines[i][2];
        }
        lastDistance = verticalLines[i][2];
    }

    distanceThreshold = warped.cols / (9*2);
    lastDistance = distanceThreshold;
    boxCounter = 0;
    // store for box position values
    Vec2f boxYValues[NO_OF_BOXES] = {0};
    for( size_t i = 0; i < horizontalLines.size(); i++ ) {
        if ((horizontalLines[i][2] - lastDistance) > distanceThreshold) {
            boxYValues[boxCounter][1] = horizontalLines[i][2];
            boxCounter++;
            if (boxCounter == 9) break;
            boxYValues[boxCounter][0] = horizontalLines[i][2];
        } else {
            boxYValues[boxCounter][0] = horizontalLines[i][2];
        }
        lastDistance = horizontalLines[i][2];
    }

    // build rects from our box knowledge
    vector<Rect> rects;
    for( size_t y = 0; y < NO_OF_BOXES; y++ ) {
        for( size_t x = 0; x < NO_OF_BOXES; x++ ) {
            rects.push_back(Rect(boxXValues[x][0] + 2, boxYValues[y][0] + 2, (boxXValues[x][1] - boxXValues[x][0]) - 2, (boxYValues[y][1] - boxYValues[y][0]) - 2));
        }
    }
    return rects;
}


void Sudoku::recognizeDigits(vector<Rect>& rects) {

	CvSVM svm;
	//svm.load("svm/svm_digitRecognition_1000_rbf_auto_8vs1_hu.xml");
    //svm.load("svm/digit_trainer.xml");
    svm.load("svm/5_test.xml");
    namedWindow("digit", 0);
    resizeWindow("Final", 200,200);

    for( size_t i = 0; i < rects.size(); i++ ) {
        Mat croppedImage;
        cout << rects[i] << endl;
        Mat dRect = debugImage(rects[i]);
        croppedImage = warped(rects[i]);

        // raise the contrast of our image to reduce noise
        Mat highContrastImage = Mat::zeros( croppedImage.size(), croppedImage.type() );
        for( int y = 0; y < croppedImage.rows; y++ ) {
            for( int x = 0; x < croppedImage.cols; x++ ) {
                for( int c = 0; c < 3; c++ ) {
                    highContrastImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( 1.7 * ( croppedImage.at<Vec3b>(y,x)[c] ));
                }
            }
        }

        Mat binaryImage;
        bitwise_not (highContrastImage, highContrastImage);
        threshold(highContrastImage, binaryImage, 100, 255, THRESH_BINARY);
        cvtColor(binaryImage, binaryImage, CV_RGB2GRAY);

        imshow("digit", binaryImage);
        waitKey(100);

        vector<vector<Point> > contours;
        findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        int largestContourIndex = 0;
        double largestContourArea = 0;

        if (contours.size() > 0) {
            for( int j = 0; j < contours.size(); j++ ) {
                double conArea = contourArea(contours[j], false);
                if (conArea > largestContourArea) {
                    largestContourArea = conArea;
                    largestContourIndex = j;
                }
            }

            float contourThreshold = ((warped.rows * warped.cols) / (NO_OF_BOXES*NO_OF_BOXES*100));
            if (largestContourArea > contourThreshold) {
                drawContours(dRect, contours, largestContourIndex, CV_RGB(0, 255, 0), 1, 8);

                Moments momnts = moments(contours[largestContourIndex]);
                double huMoments[7];
                HuMoments(momnts, huMoments);

                Mat extractedFeatures(7, 1, CV_32FC1);
                for(int k = 0; k < 7; k++) {
                    extractedFeatures.at<float>(k, 0) = huMoments[k];
                }
                int detectedClass = svm.predict(extractedFeatures);
                digits[i/9][i%9] = detectedClass;
            }
        }
    }
}

void Sudoku::printDigits() {
    for (int i = 0; i < NO_OF_BOXES; ++i) {
        if (i%3 == 0) {
            cout << "|______|______|______" << endl;
        }
        for (int j = 0; j < NO_OF_BOXES; ++j) {
            if (j%3 == 0) {
                cout << "|";
            }
            cout << digits[i][j] << ' ';
        }
        cout << endl;

    }
}

void Sudoku::checkUserFeedback() {
    imshow("sudoku", warped);
    waitKey(100);

    int row, col, digit;
    string isCorrect = "n";
    while (isCorrect == "n") {
        printDigits();
        cout << "Wurden die Ziffern korrekt erkannt? (y/n)";
        cin >> isCorrect;
        cout << isCorrect << endl;
        if (isCorrect == "n") {
            cout << "Zeile? ";
            cin >> row;
            cout << "Spalte? ";
            cin >> col;
            cout << "Ziffer? ";
            cin >> digit;

            digits[row-1][col-1] = digit;
        } else {
            break;
        }
    }
}

void Sudoku::drawLines(Mat& image, vector<Vec2f>& lines) {

	for( size_t i = 0; i < lines.size(); i++ ) {

        float rho = lines[i][0];
        float theta = lines[i][1];

        double a = cos(theta);
		double b = sin(theta);

        double x0 = a*rho;
		double y0 = b*rho;

        // get two points on every line to draw it
        Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)), cvRound(y0 - 1000*(a)));

        line(image, pt1, pt2, Scalar(0,0,255), 1, 8);

    }

}

void Sudoku::drawRects(Mat& image, vector<Rect>& rects) {

    for( size_t i = 0; i < rects.size(); i++ ) {
        rectangle(image, rects[i], Scalar(255,0,0));
    }

}

float Sudoku::roundToPlaces(float val, int places) {
    int pot = pow(10, places);
    return floorf(val * pot) / pot;
}

Sudoku::~Sudoku() {
    //dtor
}
