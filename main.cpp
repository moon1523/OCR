#include <iostream>
#include <iomanip>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

#include "ScreenShot.hh"

using namespace std;
using namespace cv;

void InvertColor (Mat& img, Mat& inverted_img);
void PrintUsage()
{

}

int main(int argc, char* argv[])
{
	string outText;

//	int margin = 120;
//	int res_x = 1920;
//	int res_y = 1080;
//	int left = res_x + margin/2;
//	int top = margin/2;
//	int width = res_x - margin;
//	int height = res_y - margin;

	int left   = atoi(argv[1]);
	int top    = atoi(argv[2]);
	int width  = atoi(argv[3]);
	int height = atoi(argv[4]);

	ScreenShot screen(left,top,width,height);
	cv::Mat img;
	cv::Mat inverted_img;
	cv::Mat magnified_img;
	cv::Mat grayscale_img;
	cv::Mat binary_img;
	screen(img);
	cv::imshow("img", img);
	InvertColor(img, inverted_img);
//	cv::imshow("invertead_img", inverted_img);
//	img = inverted_img;

//	 Magnify image for better recognition
//	resize(img, magnified_img, Size(width*2.5, height*2.5), 0, 0, INTER_CUBIC);
//	cv::imshow("magnified_img", magnified_img);
//	img = magnified_img;

	//	InvertColor(img, inverted_img);
	//	cv::imshow("invertead_img", inverted_img);
	//	img = inverted_img;
	//
	//	cv::cvtColor(inverted_img, grayscale_img, COLOR_BGR2GRAY);
	//	cv::imshow("grayscale_img", grayscale_img);
	//	cv::threshold(grayscale_img, binary_img, 200, 255, THRESH_BINARY);
	//	cv::imshow("binary_img", binary_img);
	//	img = binary_img;


	cv::waitKey(0);

	tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI(); // Create Tesseract object
	ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);           // Initialize tesseract to use English (eng) and the LSTM OCR engine.
	ocr->SetPageSegMode(tesseract::PSM_AUTO_OSD);               // Set Page segmentation mode to PSM_AUTO_OSD
	ocr->SetImage(img.data, img.cols, img.rows, 3, img.step);   // Set image data
	outText = string(ocr->GetUTF8Text());                       // Run Tesseract OCR on image
	cout << ">> Sample OCR ===================================" << endl;
	cout << outText << endl;                                     // Print
	cout << "=================================================" << endl;
	ocr->End();                                                 // Destroy used object and release memory

	cout << "Press any enter in the command window to continue read screen capture" << endl;
	getchar();

	while(1) {
		ifstream ifs; ofstream ofs;
		string dump;

		ofs.open("tmp.txt");
		tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI(); // Create Tesseract object
		ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);           // Initialize tesseract to use English (eng) and the LSTM OCR engine.
		ocr->SetPageSegMode(tesseract::PSM_AUTO_OSD);               // Set Page segmentation mode to PSM_AUTO_OSD
		ocr->SetImage(img.data, img.cols, img.rows, 3, img.step);   // Set image data
		outText = string(ocr->GetUTF8Text());                        // Run Tesseract OCR on image
		cout << ">> OCR Result ===================================" << endl;
		cout << outText << endl;                                     // Print
		cout << "=================================================" << endl;
		ocr->End();                                                 // Destroy used object and release memory


//		ifs.open("tmp.txt");
//		while(ifs >> dump) {
//			cout << ">> OCR Result ===================================" << endl;
//			cout << dump << endl;
//			cout << "=================================================" << endl;
//		}

		img.release();
		screen(img);
//		resize(img, magnified_img, Size(width*2.5, height*2.5), 0, 0, INTER_CUBIC);
//		img = magnified_img;
		ofs.close();
		ifs.close();
	}
	cout << endl;





	return EXIT_SUCCESS;
}

void InvertColor (Mat& img, Mat& inverted_img)
{
	CV_Assert(img.type() == CV_8UC4);
	inverted_img = img.clone();
	for(int i=0;i<img.rows;i++) {
		for (int j=0;j<img.cols;j++) {
			for (int k=0;k<img.channels();k++) {
				inverted_img.at<Vec4b>(i,j)[k] = 255 - inverted_img.at<Vec4b>(i,j)[k];
			}
		}
	}
}
