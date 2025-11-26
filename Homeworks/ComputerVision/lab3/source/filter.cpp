#define _USE_MATH_DEFINES

#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <math.h> 
#include "filter.h"


using namespace cv;
using namespace std;
// constructor
Filter::Filter(Mat input_img, int size) {

	input_image = input_img;
	if (size % 2 == 0)
		size++;
	filter_size = size;
}

// for base class do nothing (in derived classes it performs the corresponding filter)
void Filter::doFilter() {

	// it just returns a copy of the input image
	result_image = input_image.clone();

}

// get output of the filter
cv::Mat Filter::getResult() {

	return result_image;
	}

//set window size (it needs to be odd)
void Filter::setSize(int size) {

	if (size % 2 == 0)
		size++;
	filter_size = size;
}

//get window size 
int Filter::getSize() {

	return filter_size;
}

MedianFilter::MedianFilter(Mat in, int size) : Filter(in, size) {}

void MedianFilter::doFilter()
{
	medianBlur(input_image, result_image, filter_size);
	/*
	Mat gray = input_image;
	int median0, median1, median2;
	vector<int> kernel0, kernel1, kernel2;
	for (int i = 1; i < gray.rows-filter_size; i++)
	{
		for (int j = 1; j < gray.cols-filter_size; j++)
		{
			median0 = 0;
			median1 = 0;
			median2 = 0;
			kernel1.clear();
			kernel2.clear();
			kernel0.clear();
			for (int x = i; x < i + filter_size; x++)
			{
				for (int y = j; y < j + filter_size; y++)
				{
					kernel0.push_back(gray.at<Vec3b>(x, y)[0]);
					kernel1.push_back(gray.at<Vec3b>(x, y)[1]);
					kernel2.push_back(gray.at<Vec3b>(x, y)[2]);
				}
			}
			sort(kernel0.begin(), kernel0.end());
			sort(kernel1.begin(), kernel1.end());
			sort(kernel2.begin(), kernel2.end());
			median0 = kernel0[(filter_size*filter_size)/2];
			median1 = kernel1[(filter_size*filter_size) / 2];
			median2 = kernel2[(filter_size*filter_size) / 2];
			gray.at<Vec3b>(i, j)[0] = median0;
			gray.at<Vec3b>(i, j)[1] = median1;
			gray.at<Vec3b>(i, j)[2] = median2;


		}
	}
	//cvtColor(gray, result_image, COLOR_GRAY2BGR);
	result_image = gray;
	cout << "filter finished" << endl;*/
}

GaussianFilter::GaussianFilter(Mat in, int size, double sig) : Filter(in, size) 
{
	sigma = sig;
}

void GaussianFilter::doFilter()
{
	GaussianBlur(input_image, result_image, Size(filter_size, filter_size), sigma, sigma, BORDER_DEFAULT);
}

void GaussianFilter::setSigma(double s) { sigma = s; }

double GaussianFilter::getSigma() { return sigma; }

BilateralFilter::BilateralFilter(Mat in, int size, double range, double space) : Filter(in, size)
{
	sigmarange = range;
	sigmaspace = space;
}

double BilateralFilter::getSigmaspace() 
{ return sigmaspace; }

double BilateralFilter::getSigmarange() 
{ return sigmarange; }

void BilateralFilter::setSigmaspace(double s) 
{ sigmaspace = s; }

void BilateralFilter::setSigmarange(double s) 
{ sigmarange = s; }

void BilateralFilter::doFilter()
{
	bilateralFilter(input_image, result_image, filter_size, sigmarange, sigmaspace, BORDER_DEFAULT);
}

