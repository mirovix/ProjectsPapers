#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

const float d = 0.11;
const int checkerboard_x = 5;
const int checkerboard_y = 6;

//Given the intrinsic, distortion, rotation and translation matrices, we must first transform the object point to image point 
//using projectPoints(). We can calculate the absolute norm between what we got with our transformation and the corner finding algorithm.
double  reprojectionError(vector<vector<Point3f> >& object_points, vector<vector<Point2f> >& imagePoints,
	vector<Mat>& rot, vector<Mat>& tra, Mat& camera_matrix, Mat& dist_effect, vector<float>& per_view_error);

//Choose the index of elements that have the minimal/maximal error in reprojection error
void estimationBestWorst(double total_avg_error, vector<float> per_view_error, float &worst, float &best);

//Function for drawing and finding corners (for all images) 
void drawFindCheckBoard(Mat &src, string dir, Size pattern, vector<Point2f> &corners, vector <vector <Point3f >> &points3d,
	vector <vector <Point2f >> &points2d, vector <Point3f> &obj_point3d);

//Test robot
void test(Mat img1, Mat cam_matrix, Mat dist_effect);

//Printing the results
void printing_results(double cc, double total_avg_error, Mat cam_matrix, Mat dist_effect, float worst, float best);