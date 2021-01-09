#ifndef INCLUDE_FUNCTIONS_HH_
#define INCLUDE_FUNCTIONS_HH_

#include <iostream>
#include <termio.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace cv;
using namespace std;

void InvertColor (Mat& img, Mat& inverted_img)
{
	CV_Assert(img.type() == CV_8UC3);
	inverted_img = img.clone();
	for(int i=0;i<img.rows;i++) {
		for (int j=0;j<img.cols;j++) {
			for (int k=0;k<img.channels();k++) {
				inverted_img.at<Vec3b>(i,j)[k] = 255 - inverted_img.at<Vec3b>(i,j)[k];
			}
		}
	}
}

void Tesseract_OCR(Mat& img)
{
	string outText;
	tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI(); // Create Tesseract object
	ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);           // Initialize tesseract to use English (eng) and the LSTM OCR engine.
	ocr->SetPageSegMode(tesseract::PSM_AUTO_OSD);               // Set Page segmentation mode to PSM_AUTO_OSD
	ocr->SetImage(img.data, img.cols, img.rows, 3, img.step);   // Set image data
	outText = string(ocr->GetUTF8Text());                       // Run Tesseract OCR on image

	cout << ">> OCR Result ===================================" << endl;
	cout << outText << endl;                                     // Print
	cout << "=================================================" << endl;

	ocr->End();
}

int getch()
{
	int ch;

	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;

	buf.c_lflag &= ~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;

	tcsetattr(0, TCSAFLUSH, &buf);

	ch = getchar();

	tcsetattr(0, TCSAFLUSH, &save);

	return ch;
}

#endif /* INCLUDE_FUNCTIONS_HH_ */
