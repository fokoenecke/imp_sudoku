#include "Button.h"

using namespace cv;

Button::Button()
{
}
Button::~Button()
{
	Imgup.release();
	Imgdown.release();
}
void Button::Init(std::string i1, std::string i2, int ax, int ay,  std::string awn)
{
	x = ax; y = ay;
	wn = awn;
	Imgup = cv::imread(i1);
	xl = Imgup.size().width;
	yl = Imgup.size().height;
	xmax = x + xl; ymax = y + yl;
	ch = Imgup.channels();
	Imgdown = cv::imread(i2);
	state = down;
}
void Button::Drawup(cv::Mat& dest)
{
	Split(&Imgup, &dest, x, y, xl, yl, ch);
}
void Button::Drawdown(cv::Mat& dest)
{
	Split(&Imgdown, &dest, x, y, xl, yl, ch);
}
bool Button::handleEvent(cv::Mat& dest,int event, int mx, int my)
{
	if (InRange(mx,my,x,y,xmax,ymax)) 
    {
		if (event == EVENT_LBUTTONDOWN)
		{
			if (state == up)
			{
				state = down;
				Drawdown(dest);
				imshow(wn, dest);
			}
			return false; 
		}
		else
		if (event == EVENT_LBUTTONUP)
		{
			if (state== down) 
            {
				state = up;
				Drawup(dest);
				imshow(wn, dest);
				return true;
			}
			else { return false; }
		}
		else { return false; }
	}
	else 
    { 
      return false; 
     }
}