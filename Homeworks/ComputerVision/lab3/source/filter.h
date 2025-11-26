#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
// Generic class implementing a filter with the input and output image data and the parameters
class Filter {

	// Methods

public:

	// constructor 
	// input_img: image to be filtered
	// filter_size : size of the kernel/window of the filter
	Filter(cv::Mat input_img, int filter_size);

	// perform filtering (in base class do nothing, to be reimplemented in the derived filters)
	void doFilter();

	// get the output of the filter
	cv::Mat getResult();

	//set the window size (square window of dimensions size x size)
	void setSize(int size);

	//get the Window Size
	int getSize();

	// Data

protected:

	// input image
	cv::Mat input_image;

	// output image (filter result)
	cv::Mat result_image;

	// window size
	int filter_size;



};

class GaussianFilter : public Filter {
public:

	GaussianFilter(Mat in, int size, double sig);
	virtual void doFilter();
	void setSigma(double s);
	double getSigma();

private:
	double sigma;
};

class MedianFilter : public Filter {

public :

	MedianFilter(Mat in, int size);
	virtual void doFilter();
};

class BilateralFilter : public Filter {

public:

	BilateralFilter(Mat in, int size, double range, double space);
	double getSigmaspace();
	double getSigmarange();
	void setSigmaspace(double s);
	void setSigmarange(double s);
	virtual void doFilter();

private:

	double sigmarange;
	double sigmaspace;
};