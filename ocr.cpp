#include <iostream>
#include <iomanip>
#include <tuple>

#include "ScreenShot.hh"
#include "Functions.hh"

using namespace std;
using namespace cv;

void PrintUsage()
{
	cout << "Usage: ./ocr -cap [left] [top] [width] [height] [options]" << endl;
	cout << "[left]   : starting x pixel point" << endl;
	cout << "[top]    : starting y pixel point" << endl;
	cout << "[width]  : set the width resolution" << endl;
	cout << "[height] : set the height resolution" << endl << endl;

	cout << "[options]" << endl;
	cout << "  1.Image Editing Option" << endl;
	cout << "    -rec         : recording the screen capture sets" << endl;
	cout << "    -inv         : inverting the color of the screen capture" << endl;
	cout << "    -gry         : converting the color of screen capture to grayscale" << endl;
	cout << "    -bin [value] : screen capture image binarization [value: blocksize (odd integer) default:9]" << endl;
	cout << "    -siz [value] : resize the screen capture [value: resize factor (integer)]" << endl;
	cout << "    -rgb [RGB_Font] [RGB_Back] [value]" << endl;
	cout << "                 : Convert text and background color to white and black respectively." << endl;
	cout << "                   [RGB_Font/Back: R G B (0-255 integer)[value: threshold (0-255 integer)]" << endl;
	cout << "  2. Training" << endl;
	cout << "  3. Camera setting" << endl;
}

int main(int argc, char* argv[])
{
	bool record(false); bool invert(false); bool _resize(false); bool binary(false); bool gray(false);
	bool rgb(false);
	int font_R, font_G, font_B; int back_R, back_G, back_B;
	int left, top, width, height;
	float resize_factor(1);
	int binary_threshold(150);
	int binary_blocksize(9);

	if (argc == 1) { PrintUsage(); return 1; }
	for (int i=1;i<argc;i++) {
		// option
		if 		( string(argv[i]) == "-rec" ) {  record = true; }
		else if ( string(argv[i]) == "-inv" ) {  invert = true; }
		else if ( string(argv[i]) == "-gry" ) {    gray = true; }
		else if ( string(argv[i]) == "-bin" ) {  binary = true; binary_threshold = atoi(argv[++i]); }
			//binary_blocksize = atoi(argv[i++]); } //
		else if ( string(argv[i]) == "-siz" ) { _resize = true; resize_factor = atoi(argv[++i]); }
		else if ( string(argv[i]) == "-rgb" ) {     rgb = true;
			font_R = atoi(argv[++i]); font_G = atoi(argv[++i]); font_B = atoi(argv[++i]);
			back_R = atoi(argv[++i]); back_G = atoi(argv[++i]); back_B = atoi(argv[++i]);
		}

		// screen capture parameters
		else if ( string(argv[i]) == "-cap" ) {    left = atoi(argv[++i]);
												    top = atoi(argv[++i]);
												  width = atoi(argv[++i]);
												 height = atoi(argv[++i]); }
		else { PrintUsage(); return 1; }
	}


	cv::Mat img, inverted_img, resized_img, grayscale_img, binary_img;

	// Screen Capture
	cout << "### SET THE REGION OF INTERSET ###" << endl;
	ScreenShot screen(left,top,width,height);
	while(1) {
		screen(img);
		resize(img, resized_img, Size(width*resize_factor, height*resize_factor), 0, 0, INTER_LINEAR);
		img = resized_img;

		if ( invert) { InvertColor(img, inverted_img); img = inverted_img; }
		if (   gray) { cv::cvtColor(img, grayscale_img, COLOR_BGR2GRAY); img = grayscale_img; }
		if ( binary) {
			cv::cvtColor(img, grayscale_img, COLOR_BGR2GRAY);
//			cv::adaptiveThreshold(grayscale_img, binary_img, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 9, 10);
			cv::threshold(grayscale_img, binary_img, binary_threshold, 255, THRESH_BINARY);
			img = binary_img;
		}
		cv::imshow("img", img);
		cout << "  Press ESC key on the image to determine present screen capture" << endl;
		cout << "  If not, close the image window (Press any key except for ESC on the image window)" << endl;
		cout << "  Set screen capture position and size [left] [right] [width] [height]" << endl;
		if (cv::waitKey() == 27) {cv::destroyAllWindows(); break;}
		cv::destroyAllWindows();
		cin >> left >> top >> width >> height;
		screen.SetDisplay(left, top, width, height);
	} cout << "### COMPLETE ROI SETTING ###" << endl << endl;

//	cout << "### CONVERTED IMAGE ###" << endl;
//	cout << "  Press any key to close the window" << endl << endl;
//	cv::imshow("Converted Image", img);
//	cv::waitKey();
//	cv::destroyAllWindows();

	Tesseract_OCR(img);

	cout << "### Main loop ###" << endl;
	cout << "Start : ENTER" << endl;
	cout << "Break : ESC" << endl <<endl;
	getchar();


	int frame_count(0);

	ofstream rec; if (record) rec.open("record.mat");

	// Main loop
	while(1) {

		if (record) {
			rec << "#frame " << frame_count++ << endl;
			rec << img << endl;
		}

		ifstream ifs; ofstream ofs;
		Tesseract_OCR(img);

		img.release();
		screen(img);

		if ( invert) { InvertColor(img, inverted_img); img = inverted_img; }
		if (   gray) { cv::cvtColor(img, grayscale_img, COLOR_BGR2GRAY); img = grayscale_img; }
		if ( binary) {
			cv::cvtColor(img, grayscale_img, COLOR_BGR2GRAY);
//			cv::adaptiveThreshold(grayscale_img, binary_img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 10);
			cv::threshold(grayscale_img, binary_img, binary_threshold, 255, THRESH_BINARY);
			img = binary_img;
		}
		if (_resize) {
			resize(img, resized_img, Size(width*resize_factor, height*resize_factor), 0, 0, INTER_LINEAR);
			img = resized_img;
		}


		ofs.close();
		ifs.close();
	} // Main loop


	return EXIT_SUCCESS;
}


