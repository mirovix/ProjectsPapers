#include "include/canny.h"

using namespace cv;
using namespace std;

struct parCanny
{
	Mat img, dst, y;
	int min_value;
	int const max_value = 400;
	int max_threshold;
	int min_threshold;
	int kernel_canny = 3;
	int kernel = 3;
	int min_Kernel = 3;
	int max_Kernel = 10;
};
parCanny pars_canny;

//Set pars for Canny H_Threshold = 180, L_Threshold = 224, Kernel = 8
void setParsCanny(char* source)
{
	Mat img_gray, img = imread(source);
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	String name = "Set Values Canny";
	pars_canny.img = img_gray;
	pars_canny.y = img.clone();
	namedWindow(name, WINDOW_NORMAL);
	imshow(name, img);
	createTrackbar("High threshold :", name, &pars_canny.min_value, pars_canny.max_value, setValuesMax);
	createTrackbar("Min threshold :", name, &pars_canny.min_value, pars_canny.max_value, setValuesMin);
	createTrackbar("Sigma :", name, &pars_canny.min_Kernel, pars_canny.max_Kernel, setValuesKernel);
	waitKey(0);
}

void setValuesMax(int x, void*)
{
	pars_canny.max_threshold = x;
	Canny(pars_canny.img, pars_canny.dst, pars_canny.min_threshold, pars_canny.max_threshold, pars_canny.kernel_canny);
	imshow("Set Values Canny", pars_canny.dst);
}

void setValuesMin(int x, void*)
{
	pars_canny.min_threshold = x;
	Canny(pars_canny.img, pars_canny.dst, pars_canny.min_threshold, pars_canny.max_threshold, pars_canny.kernel_canny);
	imshow("Set Values Canny", pars_canny.dst);
}

void setValuesKernel(int x, void*)
{
	if (x < 1)
		x = 1;
	pars_canny.kernel = x;
	pars_canny.img = pars_canny.y.clone();
	blur(pars_canny.img, pars_canny.img, Size(pars_canny.kernel, pars_canny.kernel));
	Canny(pars_canny.img, pars_canny.dst, pars_canny.min_threshold, pars_canny.max_threshold, pars_canny.kernel_canny);
	imshow("Set Values Canny", pars_canny.dst);
}
