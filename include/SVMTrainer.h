#ifndef SVMTRAINER_H
#define SVMTRAINER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>

using namespace std;
using namespace cv;

class SVMTrainer
{
    public:
        SVMTrainer();
        void train();
        virtual ~SVMTrainer();
    protected:
    private:
};

#endif // SVMTRAINER_H
