#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

void setParsHoughLines(char* source);
void setValuesThreshold(int x, void*);
void setValuesRho(int x, void*);
void setValuesTheta(int x, void*);
Mat draw(vector<Vec2f> lines, Mat y);