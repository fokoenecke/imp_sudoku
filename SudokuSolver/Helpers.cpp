#include "Helpers.h"

Helpers::Helpers() {
    //ctor
}

Helpers::~Helpers() {
    //dtor
}

Point2f Helpers::computeIntersectionOfTwoLines(Vec2f l1, Vec2f l2) {
	// Punkte (x1,y1) und (x2,y2) für Linie 1 erzeugen
    float rho = l1[0];
    float theta = l1[1];
    double a = cos(theta);
	double b = sin(theta);
    double xl1 = a*rho;
	double yl1 = b*rho;
    int x1 = cvRound(xl1 + 100*(-b));
	int y1 = cvRound(yl1 + 100*(a));
    int x2 = cvRound(xl1 - 100*(-b));
    int y2 = cvRound(yl1 - 100*(a));

	// Punkte (x3,y3) und (x4,y4) für Linie 2 erzeugen
    rho = l2[0];
    theta = l2[1];
    a = cos(theta);
	b = sin(theta);
    double xl2 = a*rho;
	double yl2 = b*rho;
    int x3 = cvRound(xl2 + 100*(-b));
    int y3 = cvRound(yl2 + 100*(a));
    int x4 = cvRound(xl2 - 100*(-b));
    int y4 = cvRound(yl2 - 100*(a));


	float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

	if (d != 0) {
		Point2f pt;
		pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
		pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;

		return pt;
	}

	// Linien sind parallel
	else {
		return Point2f(-1, -1);
	}
}
