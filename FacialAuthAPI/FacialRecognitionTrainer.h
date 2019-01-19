#include <iostream>
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;

class FacialRecognitionTrainer {
public:
	static void CreateTrainerCSV(Mat, int);
	static void ReadCSVTrainingSet(const string&, vector<Mat>&, vector<int>&, int);
	void TrainFromImageSet();
	static vector<int> GetAvailableTrainingSets();
	static void CreateNewTrainingSet(int, string);
	static vector<string> RetrieveIDMatch();
};