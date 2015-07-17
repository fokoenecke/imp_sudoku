#ifndef HELPERS_H
#define HELPERS_H

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class Helpers {
    public:
        Helpers();
        virtual ~Helpers();
        static Point2f computeIntersectionOfTwoLines(Vec2f l1, Vec2f l2);
    protected:
    private:
};

#endif // HELPERS_H
