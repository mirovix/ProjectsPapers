#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "filter.h"


using namespace cv;
using namespace std;
const char* source1 = "data/image.jpg";
const char* source2 = "data/countryside.jpg";
const char* source3 = "data/lena.png";
const char* source4 = "data/tree.jpg";

struct ImageWithParamsMed
{
	int tb_max;
	int a_m;
	Mat src_img;
};
ImageWithParamsMed iwpm;

struct ImageWithParamsGauss
{
	int tb_maxs;
	int tb_maxk;
	int a_m;
	double sigma;
	int valsig;
	Mat src_img;
};
ImageWithParamsGauss iwpg;

struct ImageWithParamsBil
{
	int tb_maxCol;
	int tb_maxSpa;
	int a_m;
	double sigmaColor;
	double sigmaSpace;
	int valsigcolor;
	int valsigspace;
	Mat src_img;
};
ImageWithParamsBil iwpb;

void showHistogram(vector<Mat>& hists);
void part1();
void part2();
void foo1(int x, void*);
void foo2s(int x, void* ud);
void foo2k(int x, void* ud);
void foo3s(int x, void* ud);
void foo3c(int x, void* ud);

int main(void)
{
	part1(); // Histogram Equalization
	part2(); // Image Filtering
	return 0;
}

void part2()
{
	Mat s = imread(source4);
	String g = "gaussian";
	String m = "median";
	String b = "bilateral";

	//gaussian
	iwpg.src_img = s;
	iwpg.tb_maxs = 50;
	iwpg.tb_maxk = 50;
	iwpg.a_m = 3;
	iwpg.sigma = 0.0;
	iwpg.valsig = 0;
	namedWindow(g, WINDOW_NORMAL);
	createTrackbar("kerenl", g, &iwpg.a_m, iwpg.tb_maxk, foo2k);
	createTrackbar("sigma", g, &iwpg.valsig, iwpg.tb_maxs, foo2s);
	foo2s(iwpg.a_m, NULL);
	foo2k(iwpg.valsig, NULL);

	//median
	iwpm.src_img = s;
	iwpm.a_m = 3;
	iwpm.tb_max = 50;
	namedWindow(m, WINDOW_NORMAL);
	createTrackbar("kerenl",m, &iwpm.a_m, iwpm.tb_max, foo1);
	foo1(iwpm.a_m, NULL);

	//bilateral
	iwpb.a_m = 9;
	iwpb.src_img = s;
	iwpb.tb_maxCol = 100;
	iwpb.tb_maxSpa = 100;
	iwpb.sigmaColor = 0.0;
	iwpb.sigmaSpace = 0.0;
	iwpb.valsigcolor = 0;
	iwpb.valsigspace = 0;
	namedWindow(b, WINDOW_NORMAL);
	createTrackbar("sigmaColor", b, &iwpb.valsigcolor, iwpb.tb_maxCol, foo3c);
	createTrackbar("sigmaSpace", b, &iwpb.valsigspace, iwpb.tb_maxSpa, foo3s);
	foo3s(iwpb.valsigspace, NULL);
	foo3c(iwpb.valsigcolor, NULL);
	
	waitKey(0);
	
}

void foo3c(int x, void* ud)
{
	iwpb.sigmaColor = double(x);
	BilateralFilter bl(iwpb.src_img, iwpb.a_m, iwpb.sigmaColor, iwpb.sigmaSpace);
	bl.doFilter();
	imshow("bilateral", bl.getResult());
}

void foo3s(int x, void* ud)
{
	iwpb.sigmaSpace = double(x);
	BilateralFilter bl(iwpb.src_img, iwpb.a_m, iwpb.sigmaColor, iwpb.sigmaSpace);
	bl.doFilter();
	imshow("bilateral", bl.getResult());
}

void foo2s(int x, void* ud)
{
	iwpg.sigma = double(x);
	GaussianFilter gf(iwpg.src_img, iwpg.a_m, iwpg.sigma);
	gf.doFilter();
	imshow("gaussian", gf.getResult());
}

void foo2k(int x, void* ud)
{
	iwpg.a_m = x;
	GaussianFilter gf(iwpg.src_img, iwpg.a_m, iwpg.sigma);
	gf.doFilter();
	imshow("gaussian", gf.getResult());
}

void foo1(int x, void*)
{
	iwpm.a_m = x;
    MedianFilter mf(iwpm.src_img, iwpm.a_m);
	mf.doFilter();
	imshow("median", mf.getResult());
} 

void part1()
{
	Mat src = imread(source4); //img 
	resize(src, src, Size(src.cols / 2.5, src.rows / 2.5));
	int size = 256; //from 0 to 255
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* hrange = { range };
	//Set the range of values(as we said, between 0 and 255)
	Mat res0, res1, res2, res3;
	vector<Mat> img;
	vector<Mat> img2;
	vector<Mat> hists(3);
	vector<Mat> hists1(3);
	vector<Mat> hists2(3);
	vector<Mat> hists3(3);

	split(src, img);
	//histograms
	calcHist(&img[0], 1, 0, Mat(), hists.at(0), 1, &size, &hrange, true, false); //for blue
	calcHist(&img[1], 1, 0, Mat(), hists.at(1), 1, &size, &hrange, true, false); //for green
	calcHist(&img[2], 1, 0, Mat(), hists.at(2), 1, &size, &hrange, true, false); //for red
	//Equalize
	equalizeHist(img[0], img[0]);
	equalizeHist(img[1], img[1]);
	equalizeHist(img[2], img[2]);
	merge(img, res0); //merging
	showHistogram(hists); //show hists
	namedWindow("Source");
	imshow("Source", src);
	namedWindow("res0");
	imshow("res0", res0);

	//end of point 6

	cvtColor(src, res1, COLOR_BGR2HSV);
	split(res1, img2);
	calcHist(&img2[0], 1, 0, Mat(), hists1.at(0), 1, &size, &hrange, true, false);
	calcHist(&img2[1], 1, 0, Mat(), hists1.at(1), 1, &size, &hrange, true, false);
	calcHist(&img2[2], 1, 0, Mat(), hists1.at(2), 1, &size, &hrange, true, false);
	equalizeHist(img2[0], img2[0]);
	merge(img2, res1);
	cvtColor(res1, res1, COLOR_HSV2BGR);


	cvtColor(src, res2, COLOR_BGR2HSV);
	split(res2, img2);
	calcHist(&img2[0], 1, 0, Mat(), hists2.at(0), 1, &size, &hrange, true, false);
	calcHist(&img2[1], 1, 0, Mat(), hists2.at(1), 1, &size, &hrange, true, false);
	calcHist(&img2[2], 1, 0, Mat(), hists2.at(2), 1, &size, &hrange, true, false);
	equalizeHist(img2[1], img2[1]);
	merge(img2, res2);
	cvtColor(res2, res2, COLOR_HSV2BGR);


	cvtColor(src, res3, COLOR_BGR2HSV);
	split(res3, img2);
	calcHist(&img2[0], 1, 0, Mat(), hists3.at(0), 1, &size, &hrange, true, false);
	calcHist(&img2[1], 1, 0, Mat(), hists3.at(1), 1, &size, &hrange, true, false);
	calcHist(&img2[2], 1, 0, Mat(), hists3.at(2), 1, &size, &hrange, true, false);
	equalizeHist(img2[2], img2[2]);
	merge(img2, res3);
	cvtColor(res3, res3, COLOR_HSV2BGR);


	namedWindow("res1");
	imshow("res1", res1);
	//showHistogram(hists1);
	namedWindow("res2");
	imshow("res2", res2);
	//showHistogram(hists2);
	namedWindow("res3");
	imshow("res3", res3);
	//showHistogram(hists3);

	waitKey(0);
}

void showHistogram(std::vector<cv::Mat>& hists)
{
	// Min/Max computation
	double hmax[3] = { 0,0,0 };
	double min;
	cv::minMaxLoc(hists[0], &min, &hmax[0]);
	cv::minMaxLoc(hists[1], &min, &hmax[1]);
	cv::minMaxLoc(hists[2], &min, &hmax[2]);

	std::string wname[3] = { "blue", "green", "red" };
	cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
							 cv::Scalar(0,0,255) };

	std::vector<cv::Mat> canvas(hists.size());

	// Display each histogram in a canvas
	for (int i = 0, end = hists.size(); i < end; i++)
	{
		canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

		for (int j = 0, rows = canvas[i].rows; j < hists[0].rows - 1; j++)
		{
			cv::line(
				canvas[i],
				cv::Point(j, rows),
				cv::Point(j, rows - (hists[i].at<float>(j) * rows / hmax[i])),
				hists.size() == 1 ? cv::Scalar(200, 200, 200) : colors[i],
				1, 8, 0
			);
		}

		cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
	}
}

