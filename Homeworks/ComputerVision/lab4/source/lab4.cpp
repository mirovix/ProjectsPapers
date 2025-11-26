#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <iostream>
#include "include/hough_line.h"
#include "include/canny.h"
#include "include/hough_circle.h"

int main(int argc, char** argv)
{
	if (argc < 2)	{cout << " Usage: input.png" << endl; return -1;}
	
	//input.png
	char* source = argv[1];

	if (source == "")	{cout << "Could not open or find the image" << endl; return -1;}

	setParsCanny(source); //function used for set Canny parameters
	setParsHoughLines(source); //function used for HoughLines parameters
	setParsHoughCircle(source); //function used for HoughCircle parameters

	Mat hlines, hcircle, img = imread(source);
	vector<Vec2f> lines;
	vector<Vec3f> circles;

	//Paramiters find using the three function defined above
	int low_l = 180, high_l = 224, kernel_l = 3, size_l = 8;
	int low_c = 600, high_c = 454, kernel_c = 3, size_c = 3;
	int threshold = 101;
	double rho = 6, theta = CV_PI / 180 * 22;
	double dist = img.rows / 8, par1 = 100, par2 = 18, dp = 1;
	

	//line
	cvtColor(img, hlines, COLOR_BGR2GRAY);
	blur(hlines, hlines, Size(size_l, size_l));
	Canny(hlines, hlines, low_l, high_l, kernel_l);
	HoughLines(hlines, lines, rho, theta, threshold);
	hlines = draw(lines, img.clone());
	
	//circle
	cvtColor(img, hcircle, COLOR_BGR2GRAY);
	blur(hcircle, hcircle, Size(size_c, size_c));
	Canny(hcircle, hcircle, low_c, high_c, kernel_c);
	HoughCircles(hcircle, circles, HOUGH_GRADIENT, dp, dist, par1, par2);
	hcircle = drawCircle(circles, hlines);


	namedWindow("Result", WINDOW_AUTOSIZE);
	imshow("Result", hlines);

	waitKey(0);
	return 0;
}