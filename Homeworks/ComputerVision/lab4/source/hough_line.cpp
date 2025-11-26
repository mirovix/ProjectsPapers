#include "include/hough_line.h"

using namespace cv;
using namespace std;

struct parHoughLines {
	Mat img, source;
	int min_value = 1, max_value = 100;
	int min_v_t = 100, max_v_t = 1000;
	double rho = 1, theta = CV_PI / 180;
	int threshold = 0;
};
parHoughLines pars_hough_line;

//Set pars for HoughLines Threshold = 101, Rho = 6, Theta = PI/180*22
void setParsHoughLines(char* source)
{
	Mat img = imread(source);
	pars_hough_line.source = img;
	cvtColor(img, img, COLOR_BGR2GRAY);
	blur(img, img, Size(8, 8));
	Canny(img, img, 180, 224, 3);
	String name = "Set Values Hough Lines";
	pars_hough_line.img = img;
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, pars_hough_line.source);
	createTrackbar("Threshold :", name, &pars_hough_line.min_v_t, pars_hough_line.max_v_t, setValuesThreshold);
	createTrackbar("Rho :", name, &pars_hough_line.min_value, pars_hough_line.max_value, setValuesRho);
	createTrackbar("Theta :", name, &pars_hough_line.min_value, pars_hough_line.max_value, setValuesTheta);
	waitKey(0);
}

void setValuesTheta(int x, void*)
{
	if (x < 2)
		x = 1;
	pars_hough_line.theta = x * CV_PI / 180;
	Mat res, y = pars_hough_line.source.clone();
	vector<Vec2f> lines;
	HoughLines(pars_hough_line.img, lines, pars_hough_line.rho, pars_hough_line.theta, pars_hough_line.threshold);
	res = draw(lines, y);
	imshow("Set Values Hough Lines", res);
}

void setValuesRho(int x, void*)
{
	if (x < 2)
		x = 1;
	pars_hough_line.rho = x;
	Mat result, y = pars_hough_line.source.clone();
	vector<Vec2f> lines;
	HoughLines(pars_hough_line.img, lines, pars_hough_line.rho, pars_hough_line.theta, pars_hough_line.threshold);
	draw(lines, y);
	result = draw(lines, y);
	imshow("Set Values Hough Lines", result);
}

void setValuesThreshold(int x, void*)
{
	if (x < 50)
		x = 50;
	pars_hough_line.threshold = x;
	Mat res, y = pars_hough_line.source.clone();
	vector<Vec2f> lines;
	HoughLines(pars_hough_line.img, lines, pars_hough_line.rho, pars_hough_line.theta, pars_hough_line.threshold);
	res = draw(lines, y);
	imshow("Set Values Hough Lines", res);
}

Mat draw(vector<Vec2f> lines, Mat y)
{
	if (lines.size() != 2)
		return y;

	float rho = lines[0][0], theta = lines[0][1];
	Point pt1, pt2, pt3;
	float rho2 = lines[1][0], theta2 = lines[1][1];
	double a = cos(theta), b = sin(theta);
	double a2 = cos(theta2), b2 = sin(theta2);
	Point p[1][3];
	const Point* Points[1] = { p[0] };
	int num[] = { 3 };

	pt1.x = cvRound(((rho2 / b2) - (rho / b)) / ((-a / b) + (a2 / b2)));
	pt1.y = cvRound(((-a / b)*pt1.x) + (rho / b));

	pt2.y = cvRound(y.rows);
	pt2.x = cvRound(((pt2.y) - (rho / b)) / (-a / b));

	pt3.y = cvRound(y.rows);
	pt3.x = cvRound(((pt3.y) - (rho2 / b2)) / (-a2 / b2));

	p[0][0] = pt1;
	p[0][1] = pt2;
	p[0][2] = pt3;

	fillPoly(y, Points, num, 1, Scalar(0, 0, 255), LINE_8);
	return  y;
}