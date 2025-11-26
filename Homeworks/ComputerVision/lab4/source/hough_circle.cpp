#include "include/hough_circle.h"

using namespace cv;
using namespace std;

struct parHoughCircle {
	Mat img, source;
	int min_value = 100, max_value = 1000;
	int min_dist = 10, max_dist = 100;
	int min_dp = 1;
	int max_vDist = 300;
	double dist, par1, par2, dp;
};
parHoughCircle pars_hough_circle;

//Set pars for HoughCircle -> Canny H_Threshold = 454, L_Threshold = 600, Kernel = 3
//Set Pars for HoughCircle -> Dist = img.rows/8, par1 = 100, par2 = 18, dp = 1
void setParsHoughCircle(char* source)
{
	Mat img = imread(source);
	pars_hough_circle.source = imread(source);
	cvtColor(img, img, COLOR_BGR2GRAY);
	blur(img, img, Size(3, 3));
	Canny(img, img, 600, 454, 3);
	String name = "Set Values Hough Circle";
	pars_hough_circle.dist = img.rows / 8;
	pars_hough_circle.par1 = 200;
	pars_hough_circle.par2 = 100;
	pars_hough_circle.dp = 1;
	pars_hough_circle.img = img;
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, pars_hough_circle.source);
	createTrackbar("dist :", name, &pars_hough_circle.min_dist, pars_hough_circle.max_vDist, setValuesDist);
	createTrackbar("Par1 :", name, &pars_hough_circle.min_value, pars_hough_circle.max_value, setValuesPar1);
	createTrackbar("Par2 :", name, &pars_hough_circle.min_value, pars_hough_circle.max_value, setValuesPar2);
	createTrackbar("Dp :", name, &pars_hough_circle.min_dp, pars_hough_circle.max_dist, setValuesDp);
	waitKey(0);
}

void setValuesDist(int x, void*)
{
	pars_hough_circle.dist = pars_hough_circle.img.rows / (8 + x);
	vector<Vec3f> cir;
	HoughCircles(pars_hough_circle.img, cir, HOUGH_GRADIENT, pars_hough_circle.dp, pars_hough_circle.dist, pars_hough_circle.par1, pars_hough_circle.par2);
	Mat res = drawCircle(cir, pars_hough_circle.source.clone());
	imshow("Set Values Hough Circle", res);
}

void setValuesPar1(int x, void*)
{
	if (x < 10)
		x = 10;
	pars_hough_circle.par1 = x;
	vector<Vec3f> cir;
	HoughCircles(pars_hough_circle.img, cir, HOUGH_GRADIENT, pars_hough_circle.dp, pars_hough_circle.dist, pars_hough_circle.par1, pars_hough_circle.par2);
	Mat res = drawCircle(cir, pars_hough_circle.source.clone());
	imshow("Set Values Hough Circle", res);
}

void setValuesPar2(int x, void*)
{
	if (x < 10)
		x = 10;
	pars_hough_circle.par2 = x;
	vector<Vec3f> cir;
	HoughCircles(pars_hough_circle.img, cir, HOUGH_GRADIENT, pars_hough_circle.dp, pars_hough_circle.dist, pars_hough_circle.par1, pars_hough_circle.par2);
	Mat res = drawCircle(cir, pars_hough_circle.source.clone());
	imshow("Set Values Hough Circle", res);
}

void setValuesDp(int x, void*)
{
	if (x == 0)
		x = 1;
	pars_hough_circle.dp = x;
	vector<Vec3f> cir;
	HoughCircles(pars_hough_circle.img, cir, HOUGH_GRADIENT, pars_hough_circle.dp, pars_hough_circle.dist, pars_hough_circle.par1, pars_hough_circle.par2);
	Mat res = drawCircle(cir, pars_hough_circle.source.clone());
	imshow("Set Values Hough Circle", res);
}

Mat drawCircle(vector<Vec3f> circles, Mat y)
{
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point c(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int r = cvRound(circles[i][2]);
		circle(y, c, r, Scalar(0, 255, 0), FILLED, 8, 0);
	}
	return y;
}
