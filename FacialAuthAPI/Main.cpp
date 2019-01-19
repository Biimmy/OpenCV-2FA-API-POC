#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"
#include "opencv2/imgproc.hpp"
#include "FacialDetectionHelper.h"
#include "FacialRecognitionTrainer.h"
#include "Menu.h"

using namespace std;
using namespace cv;

int main()
{
	Mat frame;
	vector<Rect> faces;
	vector<Mat> croppedFaces;
	string imageName;

	
	int menuSelection = Menu::MainMenu();
	//FacialRecognitionTrainer::CreateTrainerCSV(frame, 0);

	//cout << "input image path to detect faces: ";
	//cin >> imageName;
	cout.flush();

	//Gathering of datasets
	if (menuSelection == 0)
	{
		//Select imageSet to Add to, or new image set
		vector<int> imageSets = FacialRecognitionTrainer::GetAvailableTrainingSets();
		int countOfSets = imageSets.size() - 1;
		//If new create new directory to store images and append to imageSets.txt
		int trainingImageSet = Menu::TrainerSelection();
		if (trainingImageSet > countOfSets)
		{
			string newSetName;
			cout << "Input name of new set" << endl;
			cin >> newSetName;
			FacialRecognitionTrainer::CreateNewTrainingSet(trainingImageSet, newSetName);
		}
		//open cap device
		VideoCapture cap(1);
		if (!cap.isOpened())
		{
			return -1;
		}
		//Pull images and detect the faces in image, and store in respective directory
		for (;;)
		{

			cap >> frame; // get a new frame from camera

			faces = FacialDetectionHelper::DetectFacesInImage(frame);
			cout << "Faces Detected: " << faces.size() << endl;
			croppedFaces = FacialDetectionHelper::CropFacesInImage(frame, faces);
			for (int i = 0; i < croppedFaces.size(); i++)
			{
				FacialRecognitionTrainer::CreateTrainerCSV(croppedFaces[i], trainingImageSet);
			}
			FacialDetectionHelper::ShowCroppedFaces(croppedFaces);
			FacialDetectionHelper::ShowEdgeDetection(frame);

			if (waitKey(30) >= 0) break;
		}
		return 1;
	}

	//Building of Model
	else if (menuSelection == 1)
	{
		cout << "Entering Model Build" << endl;
		//Instanciate images to build the model
		vector<Mat> trainingImages;
		vector<int> trainingLabels;
		vector<int> trainingSets = FacialRecognitionTrainer::GetAvailableTrainingSets();
		//Read all images in the csv file
		for (int i = 0; i < trainingSets.size(); i++)
		{
			FacialRecognitionTrainer::ReadCSVTrainingSet("trainingImages/" + to_string(trainingSets[i]) + "/", trainingImages, trainingLabels, trainingSets[i]);
		}
		//for (int i = 0; i < trainingImages.size() - 2; i++)
		//{
		//	putText(trainingImages[i], to_string(trainingLabels[i]), cvPoint(30, 30),
		//		FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
		//	imshow(to_string(i), trainingImages[i]);
		//}
		//waitKey(0);

		//build and save recognition model
		Ptr<face::LBPHFaceRecognizer> model = face::LBPHFaceRecognizer::create();
		model->train(trainingImages, trainingLabels);
		model->save("trainedModel.yml");
		model->setThreshold(65.0);
	}

	//Utilization of model to do detection
	else if (menuSelection == 2)
	{
		//instanciate and load trained model
		Ptr<face::LBPHFaceRecognizer> model = face::LBPHFaceRecognizer::create();
		model->read("trainedModel.yml");
		vector<string> predictionString = FacialRecognitionTrainer::RetrieveIDMatch();
		//model->setThreshold(65.0);

		//open video capture device to process
		VideoCapture cap(1);
		if (!cap.isOpened())
		{
			return -1;
		}
		for (;;)
		{
			//Grab frame and detect for faces
			int testLabel = 0;
			cap >> frame;
			//imshow("obj detect", frame);
			faces = FacialDetectionHelper::DetectFacesInImage(frame);
			croppedFaces = FacialDetectionHelper::CropFacesInImage(frame, faces);
			//Go through faces and identify them.
			for (int i = 0; i < croppedFaces.size(); i++)
			{
				Point faceCenter(faces[i].x + faces[i].width*.5, faces[i].y + faces[i].height*.5);

				int predicted_label = -1;
				double predicted_confidence = 0.0;
				cvtColor(croppedFaces[i],croppedFaces[i], CV_BGR2GRAY);
				model->predict(croppedFaces[i],predicted_label,predicted_confidence);
				string result_message = "C = " + to_string((int)predicted_confidence);
				putText(frame, predictionString[predicted_label], faceCenter,
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
				putText(frame, result_message, Point(faceCenter.x,faceCenter.y-20),
					FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
			}
			//overwrite mat and add rects and names of faces.
			imshow("obj detect", frame);
			
			if (waitKey(30) >= 0) break;
		}
	}
	return 1;
}