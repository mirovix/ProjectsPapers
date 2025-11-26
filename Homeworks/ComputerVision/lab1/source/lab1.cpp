#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <iostream>

#define NGBH_Y  9
#define NGBH_X  9
#define b 70
#define g 100
#define r 70

using namespace cv;
using namespace std;

const string source_img = "robot.jpg"; //same directory of the program

void onMouse(int event, int x, int y, int f, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//userdata = src
		Mat dst = *(Mat*)userdata; //cast in Mat the generic char
		if (y + NGBH_Y > dst.rows || x + NGBH_X > dst.cols)
			return;
		
		Rect rect(x, y, NGBH_X, NGBH_Y);
		Scalar mean_ = mean(dst(rect));

		Mat mask, hsv;
		cvtColor(dst, hsv, COLOR_BGR2HSV);

		inRange(hsv, Scalar(10, 100, 100), Scalar(60, 255, 255), mask);
		dst.setTo(Scalar(37, 201, 92), mask);

		cout << rect << endl;
		cout << "Mean : " << mean_ << endl;

		namedWindow("Result", WINDOW_AUTOSIZE);
		imshow("Result", dst);
		//La funzione cv :: mean () calcola il valore medio di tutti i pixel 
		//nell'array di input src che non sono mascherati. Il risultato viene calcolato
		//su base per canale se src è multicanale.
	}

}

int main(int argc, char** argv)
{

	Mat src = imread(source_img); //img 
	resize(src, src, Size(src.cols / 2, src.rows / 2));
	Mat dst = src.clone();

	namedWindow("img", WINDOW_AUTOSIZE);
	imshow("img", dst);

	setMouseCallback("img", onMouse, (char*)&dst);
	waitKey(0);
	return 0;
}
