#ifndef INCLUDE_SCREENSHOT_HH_
#define INCLUDE_SCREENSHOT_HH_

#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class ScreenShot {
public:
    ScreenShot(int x, int y, int width, int height):
        x(x),
        y(y),
        width(width),
        height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
    }

    void operator() (cv::Mat& cvImg)
    {
        if(img != nullptr)
            XDestroyImage(img);
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        cvImg = cv::Mat(height, width, CV_8UC3, img->data);
    }

    virtual ~ScreenShot()
    {
        if(img != nullptr)
            XDestroyImage(img);
        XCloseDisplay(display);
    }

    void SetDisplay(int _x, int _y, int _width, int _height) {
    	x = _x;
    	y = _y;
    	width = _width;
    	height = _height;
    }

private:
	Display* display;
	Window root;
	int x,y,width,height;
	XImage* img{nullptr};
};

#endif /* INCLUDE_SCREENSHOT_HH_ */
