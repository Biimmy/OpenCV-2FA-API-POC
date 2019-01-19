#include <iostream>
#include <fstream>
#include <sstream>
#include <direct.h>
#include "opencv2/core/core.hpp"
#include "FacialRecognitionTrainer.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


void FacialRecognitionTrainer::CreateTrainerCSV(Mat imageOfFace, int identifiedIndividualID)
{
	string trainerFilePath = "trainingImages/" + to_string(identifiedIndividualID) + "/images.txt";
	fstream trainerFile(trainerFilePath, fstream::in | fstream::out);
	string trainerFileLine;
	char separatorCharacter = ';';
	string imagePath, readIndividualID, imageNumber = "";
	int lastImageNumber = 0;

	//Will parse the file and grab the last used path name
	while (getline(trainerFile, trainerFileLine))
	{
		stringstream trainerFileLineStream(trainerFileLine);
		getline(trainerFileLineStream, imagePath);
		//getline(trainerFileLineStream, readIndividualID);
		cout << "Test image Path: " << imagePath << endl;
	}
	stringstream filePathStream(imagePath);
	getline(filePathStream, imageNumber, '.');
	if (imageNumber.compare("") != 0)
	{
		lastImageNumber = stoi(imageNumber);
		lastImageNumber++;
	}
	trainerFile.close();

	cout << "Last image number : " << lastImageNumber << endl;
	
	imshow("detectedface", imageOfFace);
	cout << "does this image contain the face to be trained to detect?(Y/N)" << endl;
	//waitKey(0);
	string newImageName = "trainingImages/" + to_string(identifiedIndividualID) + "/" + to_string(lastImageNumber) + ".png";
	string outputToCSVFile = to_string(lastImageNumber) + ".png";
	imwrite(newImageName, imageOfFace);

	ofstream output(trainerFilePath, ios::app);
	output << outputToCSVFile << endl;
	output.close();
}

void FacialRecognitionTrainer::ReadCSVTrainingSet(const string& directory, vector<Mat>& images, vector<int>& labels, int classLabel)
{
	string csv = directory + "images.txt";
	std::ifstream file(csv.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path);
		path = directory + path;
		if (!path.empty()) {
			Mat readImage = imread(path, 0);
			if (readImage.rows != 0)
			{
				images.push_back(imread(path, 0));
				labels.push_back(classLabel);
			}
		}
	}
}

vector<int> FacialRecognitionTrainer::GetAvailableTrainingSets()
{
	vector<int> availableTrainingSets;
	string trainingSetsCSV = "trainingImages/imageSets.txt";
	std::ifstream file(trainingSetsCSV.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, name, id;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, id, ';');
		getline(liness, name);
		if (!name.empty() && !id.empty()) {
			availableTrainingSets.push_back(stoi(id));
		}
	}
	return availableTrainingSets;
}

void FacialRecognitionTrainer::CreateNewTrainingSet(int id, string name)
{

	string dir = "trainingImages/" + to_string(id);
	_mkdir(dir.c_str());

	string outputToCSVFile = to_string(id) + ";" + name;

	ofstream output("trainingImages/imageSets.txt", ios::app);
	output << outputToCSVFile << endl;
}

vector<string> FacialRecognitionTrainer::RetrieveIDMatch()
{
	vector<string> idMatches;
	string trainingSetsCSV = "trainingImages/imageSets.txt";
	int traingingSetIterator = 0;
	std::ifstream file(trainingSetsCSV.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, name, id;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, id, ';');
		getline(liness, name);
		if (!name.empty() && !id.empty()) {
			idMatches.push_back(name);
		}
	}
	return idMatches;

}


