#include <iostream>
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

class FacialDetectionHelper
{
public:
	static vector<Rect> DetectFacesInImage(Mat);
	static vector<Mat> CropFacesInImage(Mat, vector<Rect>);
	static void ShowCroppedFaces(vector<Mat>);
	static void ShowEdgeDetection(Mat);
};