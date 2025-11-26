#include "lab2.h"

int main(int argc, char** argv)
{
	if (argc < 2)	{cout << " Usage: ./checkerboard_images/*.png test_image.png" << endl; return -1;}

	//./checkerboard_images/*.png - test_image.png
	string dir = argv[1];
	Mat img1 = imread(argv[2]);

	if (!img1.data || dir == " ")	{cout << "Could not open or find the image" << endl; return -1;}

	Size pattern(checkerboard_x, checkerboard_y); //Size pattern(cols, rows)
	vector<Point2f> corners, reproj_error;
	vector <vector <Point3f >> points3d;
	vector <vector <Point2f >> points2d;
	vector <Point3f> obj_point3d;
	vector<float> per_view_error;
	float worst, best;

	Mat color, src; 
	Mat cam_matrix, dist_effect;
	vector<Mat> rot, tra;
	double cc, total_avg_error;
	int total_points;

	//World coordinates
	for (int i = 0; i < checkerboard_y; i++)
		for (int j = 0; j < checkerboard_x; j++)
			obj_point3d.push_back(Point3f(j*d, i*d, 0));

	//Drawing/Finding corners
	drawFindCheckBoard(src, dir, pattern, corners, points3d, points2d, obj_point3d);

	cout << endl << "Processing..." << endl;
	destroyAllWindows();
	cc = calibrateCamera(points3d, points2d, src.size(), cam_matrix, dist_effect, rot, tra); //camera caliration
	total_avg_error = reprojectionError(points3d, points2d, rot, tra, cam_matrix, dist_effect, per_view_error);//compute reprojection error
	estimationBestWorst(total_avg_error, per_view_error, worst, best);//estimating the best and the worst pic

	//Printing
	printing_results(cc, total_avg_error, cam_matrix, dist_effect, worst, best);

	//Test Robot
	test(img1, cam_matrix, dist_effect);

	return 0;
}

void drawFindCheckBoard(Mat &src, string dir, Size pattern, vector<Point2f> &corners, vector <vector <Point3f >> &points3d,
vector <vector <Point2f >> &points2d, vector <Point3f> &obj_point3d) {

	vector<String> img;
	glob(dir, img);
	Mat color;

	for (int i = 0; i < img.size(); i++)
	{
		src = imread(img[i]);//source img
		resize(src, src, Size(src.cols / 2, src.rows / 2));//resize img
		cvtColor(src, color, COLOR_BGR2GRAY);//change color

		bool check_corner = findChessboardCorners(src, pattern, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE); //try to find corners

		if (!check_corner)//means that the function doesn't find the corners
			continue;

		cout << endl << "check_corner #" << i << " value : " << check_corner << endl;

		TermCriteria criteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 30, 0.1);
		cornerSubPix(color, corners, Size(11, 11), Size(-1, -1), criteria);

		drawChessboardCorners(src, pattern, corners, check_corner); //draw the ChessBoard

		points3d.push_back(obj_point3d);
		points2d.push_back(corners);

		namedWindow("CB", WINDOW_AUTOSIZE);
		imshow("CB", src);
		waitKey(10); //waiting for 0.01s
		cout << "End of image #" << i << endl;

	}
}

void printing_results( double cc, double total_avg_error, Mat cam_matrix, Mat dist_effect, float worst, float best) {
	cout << "-----------------------------------------------------------" << endl << "RESULTS:";
	cout << endl << "Reprojection error : " << total_avg_error << endl;
	cout << "Value of calibrate camera : " << cc << endl;
	cout << "Value of focal (X),(Y) : (" << cam_matrix.at<double>(0, 0) << "),(" << cam_matrix.at<double>(1, 1) << ")" << endl;
	cout << "Value of optical centers (X),(Y) : (" << cam_matrix.at<double>(0, 2) << "),(" << cam_matrix.at<double>(1, 2) << ")" << endl;
	cout << "Distortion coefficients = (k1 = " << dist_effect.at<double>(0) << " , k2 = " << dist_effect.at<double>(1);
	cout << " , p1 = " << dist_effect.at<double>(2);
	cout << ", p2 = " << dist_effect.at<double>(3) << ", k3 = " << dist_effect.at<double>(4) << ")" << endl;
	cout << "Best img : img[" << int(best) << "]" << endl;
	cout << "Worst img : img[" << int(worst) << "]" << endl;
}

void test(Mat img1, Mat cam_matrix, Mat dist_effect) {
	
	Mat dst1, map1, map2, new_cam_matrix, dst2, result;
	resize(img1, img1, Size(img1.cols / 2, img1.rows / 2));//resize img

	initUndistortRectifyMap(cam_matrix, dist_effect, Mat(), new_cam_matrix, Size(img1.cols, img1.rows), CV_32FC1, map1, map2);
	remap(img1, dst1, map1, map2, INTER_CUBIC);

	resize(img1, img1, Size(img1.cols / 2, img1.rows / 2));
	resize(dst1, dst1, Size(dst1.cols / 2, dst1.rows / 2));
	hconcat(img1, dst1, result);
	imshow("Result", result);
	waitKey(0);
}

void estimationBestWorst(double total_avg_error, vector<float> per_view_error, float &worst, float &best)
{
	worst = 0; best = 0;
	float max = per_view_error[worst];
	float min = per_view_error[best];
	for (int i = 0; i < per_view_error.size(); i++)
	{
		if (per_view_error[i] > max)
		{
			max = per_view_error[i];
			best = i;
		}
		if (per_view_error[i] < min)
		{
			min = per_view_error[i];
			worst = i;
		}
	}

}

double  reprojectionError(vector<vector<Point3f> >& object_points, vector<vector<Point2f> >& imagePoints,
	vector<Mat>& rot, vector<Mat>& tra, Mat& camera_matrix, Mat& dist_effect, vector<float>& per_view_error)
{
	vector<Point2f> image_points;
	int total_points = 0, num_obj_points = 0;
	double total_error = 0.0, error_temp = 0.0;
	per_view_error.resize(object_points.size());

	for (int i = 0; i < (int)object_points.size(); i++)
	{
		projectPoints(Mat(object_points[i]), rot[i], tra[i], camera_matrix, dist_effect, image_points);
		error_temp = norm(Mat(imagePoints[i]), Mat(image_points), NORM_L2);
		num_obj_points = (int)object_points[i].size();
		per_view_error[i] = (float)sqrt(error_temp*error_temp / num_obj_points);
		total_error += error_temp * error_temp;
		total_points += num_obj_points;
	}

	return sqrt(total_error / total_points);
}
