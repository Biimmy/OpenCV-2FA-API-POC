#include <iostream>
#include "FacialDetectionHelper.h"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

vector<Rect> FacialDetectionHelper::DetectFacesInImage(Mat inputImage)
{
	vector<Rect> faces;
	CascadeClassifier faceCascadeClassifier;
	faceCascadeClassifier.load("haarcascade_frontalface_default.xml");
	faceCascadeClassifier.detectMultiScale(inputImage, faces, 1.1, 2, 0, Size(140, 140));
	
	return faces;
}

vector<Mat> FacialDetectionHelper::CropFacesInImage(Mat inputImage, vector<Rect> facesVector)
{
	vector<Mat> croppedFaceImages;
	for (int i = 0; i < facesVector.size(); i++)
	{
		Point center(facesVector[i].x + facesVector[i].width*0.5, facesVector[i].y + facesVector[i].height*0.5);
		Rect facialRangeOfInterest(center.x - (facesVector[i].width*0.5), center.y - (facesVector[i].height*0.5), facesVector[i].width, facesVector[i].height);

		Mat frameRangeOfInterest = inputImage(facialRangeOfInterest);

		croppedFaceImages.push_back(frameRangeOfInterest);
	}
	return croppedFaceImages;
}

void FacialDetectionHelper::ShowCroppedFaces(vector<Mat> imagesVector)
{
	for (int i = 0; i < imagesVector.size(); i++)
	{
		imshow("Debugger image detected: " + to_string(i), imagesVector[i]);
	}
}


//Show canny edge detection for debugging asisistance
void FacialDetectionHelper::ShowEdgeDetection(Mat inputImage)
{
	Mat edges;
	Canny(inputImage, edges, 100, 200);
	imshow("edges", edges);
}


