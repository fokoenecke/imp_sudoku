#include "SVMTrainer.h"

SVMTrainer::SVMTrainer() {
    //ctor
}

void SVMTrainer::train() {

    int DIGIT_SIZE = 200;
    Mat img_color = imread("img/digits_sans_big2.png", CV_LOAD_IMAGE_COLOR);
    Mat img = imread("img/digits_sans_big2.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat binaryImg;

    bitwise_not ( img, img);
    threshold(img, binaryImg, 100, 255, THRESH_BINARY);

    int digit_cols = img.cols/DIGIT_SIZE;
    int digit_rows = img.rows/DIGIT_SIZE;
    int num_digits = digit_rows*digit_cols;

    Mat trainingDataMat(num_digits, 7, CV_32FC1);
    Mat labelsMat(num_digits, 1, CV_32FC1);

    cout << digit_rows << endl;

    for (int row = 0; row < digit_rows; row++) {
        for (int col = 0; col < digit_cols; col++) {
            int digit_index = row * digit_cols + col;

            Rect tmpRect = Rect(col*DIGIT_SIZE, row*DIGIT_SIZE, DIGIT_SIZE, DIGIT_SIZE);
            rectangle(img_color, tmpRect, Scalar(0,255,0));

            Mat debugImage;
            debugImage = img_color(tmpRect);
            Mat croppedImage;
            croppedImage = binaryImg(tmpRect);

            vector<vector<Point> > contours;

            Mat contourMat = croppedImage.clone();
            findContours(contourMat, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            int largestContourIndex = 0;

            double largestContourArea = 0;
            if (contours.size() > 0) {
                for( int i = 0; i< contours.size(); i++ ) {
                    double tmpContourArea = contourArea(contours[i], false);
                    if (tmpContourArea > largestContourArea) {
                        largestContourArea = tmpContourArea;
                        largestContourIndex = i;
                    }
                }
            }

            drawContours(debugImage, contours, largestContourIndex, CV_RGB(255, 0, 0), 1, 8);
            drawContours(croppedImage, contours, largestContourIndex, CV_RGB(255, 0, 0), 1, 8);

            Rect rect = boundingRect(contours[largestContourIndex]);
            Moments digit_moments = moments(contours[largestContourIndex]);

            double huMoments[7];
            HuMoments(digit_moments, huMoments);

            Mat extractedFeatures(1, 7, CV_32FC1);
            for(int i = 0; i < 7; i++) {
                trainingDataMat.at<float>(digit_index, i) = huMoments[i];

            }
            //labelsMat.at<float>(digit_index, 0) = (int)(row/5);
            labelsMat.at<float>(digit_index, 0) = row;

            std::ostringstream oss;
            string debug = "debug";
            oss << digit_index;
            debug += oss.str();

            namedWindow(debug);
            moveWindow(debug, 20*digit_index, 100);
            imshow(debug, croppedImage);
        }
    }


    namedWindow("test");
    moveWindow("test", 100, 100);
    imshow("test", img_color);

    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);

    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
    SVM.save("svm/digit_trainer.xml");
}

SVMTrainer::~SVMTrainer() {
    //dtor
}
