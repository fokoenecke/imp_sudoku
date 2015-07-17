#include "includes.h"
// Hilfroutinen ausserhalb der Objekte
void Split(Mat* src, Mat* dest,	int xd, int  yd, int xl, int yl, int channel)
{
	int k; int l;
	for (int i = 0, k = yd; i<(yl); i++, k++)
		for (int j = 0, l = xd; j<(xl); j++, l++)
		{
		for (int c = 0; c<channel; c++)
		{
			dest->at<uchar>(k, l*channel + c) = src->at<uchar>(i, j*channel + c);
		}
		}
}

bool InRange(int x, int y, int x1, int y1, int x2, int y2)
{
	if ((x >= x1) && (x <= x2) && (y <= y2) && (y >= y1))
	{
		return true;
	}
	else
	{
		return false;
	}
}
