#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <cmath>        
#include <cstdio>
#include <ctime>

#include "loadData.h"
#include "GameState.h"

using namespace std;
vector<double> variableScores;
int BUCKETS = 100;


inline int vectorFindB(vector<pair<char, int> >* in, char find)
{
	//this might mess up in other locations
	return (in->begin() + find)->second;
}


//this is getting the correct answers right now
void setScore(GameState* in)
{
	in->score1 = 0;
	in->score2 = 0;
	for (int c = 0; c < 20; c++)
	{
		in->score1 += vectorFindB(in->vUnitsP1, c + in->playerOffset1) * variableScores[c + in->playerOffset1];
		in->score2 += vectorFindB(in->vUnitsP2, c + in->playerOffset2) * variableScores[c + in->playerOffset2];
	}
}

//this should only be called once per main loop on the original data
pair<int, int> scoreGames(vector<GameState*>* inGameState, int minute)
{
	//only need to do 1 multiplication per game instead of 60
	int correct = 0;
	int number = 0;

	for (size_t count = 0; count < inGameState->size(); count++)
	{
		setScore((*inGameState)[count]);

		double diff = (*inGameState)[count]->score1 - (*inGameState)[count]->score2;
		(*inGameState)[count]->diff = diff;

		if (minute == (*inGameState)[count]->minute || minute == 0)
		{
			if ((*inGameState)[count]->diff > 0 && (*inGameState)[count]->winner)
				correct++;
			if ((*inGameState)[count]->diff < 0 && !(*inGameState)[count]->winner)
				correct++;

			number++;
		}
	}

	return make_pair(correct, number);
}

//take the training percent and put it into the training set and the rest go into the test set
pair<vector<GameState*>*, vector<GameState*>*> getTrainingSet(vector<GameState*>* loadedStates, int percentTraining)
{
	vector<GameState*>* trainingSet = new vector<GameState*>();
	vector<GameState*>* testSet = new vector<GameState*>();

	int numberOfGames = 0;
	//quick count for number of games, number of states with a minute = 4
	for (size_t count = 0; count < loadedStates->size(); ++count)
	{
		if ((*loadedStates)[count]->minute == 4)
			++numberOfGames;
	}

	int numInTraining = numberOfGames*percentTraining / 100;

	numberOfGames = 0;
	for (size_t count = 0; count < loadedStates->size(); ++count)
	{
		//increment at the beginning of the game to ensure the previous game finishes
		if (numberOfGames < numInTraining && (*loadedStates)[count]->minute == 4)
			++numberOfGames;

		if (numberOfGames < numInTraining)
			trainingSet->push_back((*loadedStates)[count]);
		else
			testSet->push_back((*loadedStates)[count]);
	}
	return make_pair(trainingSet, testSet);
}

void setVariables(string inputFile)
{
	ifstream in(inputFile);
	string line = "";
	//skip the header line
	getline(in, line);

	getline(in, line);
	variableScores[0] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[1] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[2] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[3] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[4] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[5] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[6] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[7] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[8] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[9] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[10] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[11] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[12] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[13] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[14] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[15] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[16] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[17] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[18] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[19] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[20] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[21] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[22] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[23] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[24] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[25] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[26] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[27] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[28] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[29] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[30] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[31] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[32] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[33] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[34] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[35] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[40] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[41] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[42] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[43] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[44] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[45] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[46] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[47] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[48] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[49] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[50] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[51] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[52] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[53] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[54] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[55] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[56] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[57] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[58] = atof(line.substr(line.find(",") + 1).c_str());
	getline(in, line);
	variableScores[59] = atof(line.substr(line.find(",") + 1).c_str());
}

int main(int argc, const char* args[])
{
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(4);

	//arguments for this
	//file input, league, number of iterations (min 10), % training

	string fileInput = "";
	string weightFile = "";
	int league = -1;
	int trainingSize = 75;

	if (argc < 3)
	{
		cout << "the required format is: " << endl;
		cout << "Project2.exe dataFile weightFile league trainingSetSize" << endl;
		cout << "trainingSetSize - min 5 max 95 default 75" << endl;
		return -1;
	}
	else
	{
		fileInput = args[1];
		weightFile = args[2];
	}
	if (argc >= 4)
		league = atoi(args[3]);
	if (argc >= 4)
		trainingSize = atoi(args[4]);

	if (trainingSize > 95)
		trainingSize = 95;
	if (trainingSize < 5)
		trainingSize = 5;

	variableScores.resize(60);
	setVariables(weightFile);

	std::cout << "Reading in data" << endl;
	//the current GameState is costly, think of a good way to keep the cost down
	//load data used to calculate the base score, now it needs to calculate it between here and the main loop
	vector<GameState*>* loadedStates = loadData(fileInput, league);
	std::cout << "Done reading data" << endl;

	pair<vector<GameState*>*, vector<GameState*>*> trainAndTest = getTrainingSet(loadedStates, trainingSize);
	vector<GameState*>* trainingSet = trainAndTest.first;
	vector<GameState*>* testingSet = trainAndTest.second;

	cout << "Test and training sets extracted." << endl;

	cout << weightFile << endl;
	//establish the correct score values for each player for every minute between 1 and 120
	for (int c = 4; c <= 30; c++)
	{
		pair<int, int> corGames = scoreGames(loadedStates, c);
		if (corGames.first > 0)
		{
			cout << c << "," << corGames.first << "," << corGames.second << endl;
		}
	}




	//end of program get rid of the actual data
	for (int count = (int)loadedStates->size() - 1; count >= 0; count--)
	{
		delete (*loadedStates)[count];
	}
	delete loadedStates;

}

