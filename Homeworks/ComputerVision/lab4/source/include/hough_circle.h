#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

void setParsHoughCircle(char* source);
void setValuesDist(int x, void*);
void setValuesPar1(int x, void*);
void setValuesPar2(int x, void*);
void setValuesDp(int x, void*);
Mat drawCircle(vector<Vec3f> circles, Mat y);