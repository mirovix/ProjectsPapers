#include "panormaic_utils.h"

using namespace cv;
using namespace std;

vector<int> matchPair(vector<Mat> imgs, double ratio);
Mat drawImage(vector<Mat> imgs, vector<int> avgTrans);

int main(int argc, char** argv)
{
	if (argc < 4) { cout << " Usage: C:\file\data\kitchen\*.bmp 5 27" << endl; return -1; }

	//C:\file\data\kitchen*.bmp or C:/file/data/dolomites/*.png
	char* dir = argv[1];
	double ratio = stod(argv[2]);
	double fov = stod(argv[3]);; //33 for dolomites - 27 the others

	if (dir == "") { cout << "Could not open or find the image" << endl; return -1; }
	if (ratio > 20 || ratio < 3) { cout << "Ratio is > 20 or < 3" << endl; return -1; }
	if (fov != 27 && fov != 33) { cout << "FOV different from 33 or 27" << endl; return -1; }

	vector<String> source;
	vector<Mat> imgs;
	glob(dir, source);

	//load images
	for (int i = 0; i < source.size(); i++)
	{
		Mat img = PanoramicUtils::cylindricalProj(imread(source[i]), fov);
		imgs.push_back(img);
		cout << "loading image : " << source[i] << endl << endl;
	}

	vector<int> avgtrs = matchPair(imgs, ratio);
	Mat final_image = drawImage(imgs, avgtrs);
	equalizeHist(final_image, final_image);

	namedWindow("Panorama Image", WINDOW_NORMAL);
	imshow("Panorama Image", final_image);
	waitKey(0);
	return 0;
}

vector<int> matchPair(vector<Mat> imgs, double ratio)
{
	vector<int> result; 
	double dist;
	result.push_back(0);

	for (int i = 0; i < imgs.size()-1; i++)
	{
		vector<KeyPoint> inliers1, inliers2;
		vector<DMatch>  good_matches;
		
		Mat first_image = imgs[i];
		Mat second_image = imgs[i + 1];

		//keypoints
		Ptr<FeatureDetector> detector = SIFT::create();
		vector<KeyPoint> keypoints1, keypoints2;
		detector->detect(first_image, keypoints1);
		detector->detect(second_image, keypoints2);

		//descriptors
		Ptr<DescriptorExtractor> extractor = SIFT::create();
		Mat descriptors1, descriptors2;
		extractor->compute(first_image, keypoints1, descriptors1);
		extractor->compute(second_image, keypoints2, descriptors2);

		//matching
		BFMatcher matcher(NORM_L2, true);
		vector<DMatch> matches;
		matcher.match(descriptors1, descriptors2, matches);

		//find min distance
		double min = matches[0].distance;
		for (int j = 1; j < matches.size(); j++)//found the min matches
		{
			dist = matches[j].distance;
			if (dist < min)
				min = dist;
		}

		//save only 'good' matches
		for (int j = 0; j < matches.size(); j++) 
			if (matches[j].distance < ratio * min) //ratio -> 4 
			{
				dist = matches[j].distance;
				good_matches.push_back(matches[j]);
			}

		//put inside obj1, obj2 points
		vector< Point2f > obj1, obj2;
		for (int j = 0; j < good_matches.size(); j++)
		{
			obj1.push_back(keypoints1[good_matches[j].queryIdx].pt);
			obj2.push_back(keypoints2[good_matches[j].trainIdx].pt);
		}

		//find a mask
		Mat fhmask;
		Mat fh = findHomography(obj1, obj2, RANSAC, ratio, fhmask);
		
		//chose only inliers
		for (int j = 0; j < fhmask.rows;j++)
		{ 
			if (fhmask.at<uchar>(j,0) > 0.0) 
			{
				for (int k = 0; k < keypoints1.size(); k++)
				{
					if (keypoints1[k].pt == obj1[j])
					{
						inliers1.push_back(keypoints1[k]);
						break;
					}
				}
				for (int k = 0; k < keypoints2.size(); k++)
				{
					if (keypoints2[k].pt == obj2[j])
					{
						inliers2.push_back(keypoints2[k]);
						break;
					}
				}
			}
		}
		
		//avg distance 
		double dx = 0;
		for (int j = 0; j < inliers1.size(); j++)
			dx += inliers1[j].pt.x - inliers2[j].pt.x;
		if (dx == 0)  cout << "Error, inliers.size() = 0" << endl << endl;

		//result 
		result.push_back((int)(dx / ((double)inliers1.size())));

		cout << "image :" << i << " and image :" << i + 1 << " are computed" << endl << endl;

	}
	return result;

}

Mat drawImage(vector<Mat> imgs, vector<int> avgTrans)
{
	cout << "...Drawing" << endl << endl;
	int total = 0;

	for (int j = 0; j < avgTrans.size(); total += avgTrans[j], j++); //total size

	Mat result = Mat::zeros(Size(total + imgs[0].cols, imgs[0].rows), CV_8UC1); //create final image (empty)

	for (int j = imgs.size() - 1; j >= 0; total -= avgTrans[j],j--)
		imgs[j].copyTo(result(Range(0, imgs[j].rows), Range(total, imgs[j].cols + total))); //put image j on final image 

	resize(result, result, Size(result.cols, result.rows/2)); 

	return result;
}
