#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/core/core.hpp>
using namespace cv;

class Detector
{
    public:
        Detector(Mat*);
        virtual ~Detector();
    protected:
    private:
};

#endif // DETECTOR_H
