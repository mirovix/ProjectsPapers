#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <iostream>

using namespace cv;
using namespace std;

void setValuesMax(int x, void*);
void setValuesMin(int x, void*);
void setValuesKernel(int x, void*);
void setParsCanny(char* source);