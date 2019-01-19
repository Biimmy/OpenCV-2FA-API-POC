#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Menu.h"

using namespace std;

int Menu::MainMenu()
{
	int input;
	string function[] = { "Generate DataSet", "Generate Model" };

	cout << "Select a function:" << endl;
	for (int i = 0; i < 2; i++)
	{
		cout << function[i] << "(" << i << ")" << endl;
	}
	cin >> input;

	return input;
}

int Menu::TrainerSelection()
{
	int trainingSet;

	cout << "Select training set to add to" << endl;

	cin >> trainingSet;

	return trainingSet;
}