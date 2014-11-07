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

#include "GameState.h"
#include "loadData.h"

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
pair<int, int> scoreGames(vector<GameState*>* inGameState)
{
	//only need to do 1 multiplication per game instead of 60
	int correct = 0;
	int number = 0;

	for (size_t count = 0; count < inGameState->size(); count++)
	{
		setScore((*inGameState)[count]);

		double diff = (*inGameState)[count]->score1 - (*inGameState)[count]->score2;
		(*inGameState)[count]->diff = diff;
		if ((*inGameState)[count]->diff > 0 && (*inGameState)[count]->winner)
			correct++;
		if((*inGameState)[count]->diff < 0 && !(*inGameState)[count]->winner)
			correct++;

		number++;
	}

	return make_pair(correct, number);
}

//this third version likely isn't necessary but its faster than the full version
pair<int, int> scoreGames(vector<GameState*>* inGameState, int minute)
{
	//only need to do 1 multiplication per game instead of 60
	int correct = 0;
	int number = 0;

	for (size_t count = 0; count < inGameState->size(); count++)
	{
		(*inGameState)[count]->diff = (*inGameState)[count]->score1 - (*inGameState)[count]->score2;
		if (((*inGameState)[count]->diff > 0 && (*inGameState)[count]->winner) || ((*inGameState)[count]->diff < 0 && !(*inGameState)[count]->winner))
		{
			//count as correct
			if (minute == 0 || minute == (*inGameState)[count]->minute)
				correct++;
		}
		if (minute == 0 || minute == (*inGameState)[count]->minute)
			number++;
	}

	return make_pair(correct, number);
}


pair<int, int> scoreGames(vector<GameState*>* inGameState, int minute, int changedVar, double prevScore, bool save)
{
	//only need to do 1 multiplication per game instead of 60

	changedVar = changedVar / 2;
	int correct = 0;
	int number = 0;

	//this further simplifies the change in score calculation by doing the subtractions outside the loop
	double changed = variableScores[changedVar] - prevScore;

	size_t stateSize = inGameState->size();

	for (size_t count = 0; count < stateSize; ++count)
	{
		//this is a factorized version of	totalscore - previous variable score + new varible score
		double P1score = (*inGameState)[count]->score1 + vectorFindB((*inGameState)[count]->vUnitsP1, changedVar) * changed;
		double P2score = (*inGameState)[count]->score2 + vectorFindB((*inGameState)[count]->vUnitsP2, changedVar) * changed;

		if (save) {
			(*inGameState)[count]->score1 = P1score;
			(*inGameState)[count]->score2 = P2score;
			(*inGameState)[count]->diff = P1score - P2score;
		}

		if ((P1score > P2score && (*inGameState)[count]->winner) || (P1score < P2score && !(*inGameState)[count]->winner))
		{
			//count as correct
			if (minute == 0 || minute == (*inGameState)[count]->minute)
				++correct;
		}
		if (minute == 0 || minute == (*inGameState)[count]->minute)
			++number;
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

int main(int argc, const char* args[])
{
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(4);

	//arguments for this
	//file input, league, number of iterations (min 10), % training

	string fileInput = "";
	int league = -1;
	int iterations = 20;
	int trainingSize = 75;

	if (argc < 3)
	{
		cout << "the required format is: " << endl;
		cout << "Project2.exe inputFile league number_of_iterations trainingSetSize matchups" << endl;
		cout << "number_of_iterations - min 10, default 20" << endl;
		cout << "trainingSetSize - min 5 max 95 default 75" << endl;
		cout << "matchups - e.g. TvZ, PvP   default none" << endl;
		return -1;
	}
	else
	{
		fileInput = args[1];
		league = atoi(args[2]);
	}
	if (argc >= 4)
		iterations = atoi(args[3]);
	if (argc >= 5)
		trainingSize = atoi(args[4]);

	if (iterations < 10)
		iterations = 10;
	if (iterations > 50)
		cout << "You are going to be here a while" << endl;

	if (trainingSize > 75)
		trainingSize = 75;
	if (trainingSize < 5)
		trainingSize = 5;

	//create the variables
	variableScores.resize(60);
	//seed the random number generator
	srand((unsigned int)time(NULL));

	std::cout << "Reading in data" << endl;
	//the current GameState is costly, think of a good way to keep the cost down
	//load data used to calculate the base score, now it needs to calculate it between here and the main loop
	vector<GameState*>* loadedStates = loadData(fileInput, league);
	std::cout << "Done reading data" << endl;

	pair<vector<GameState*>*, vector<GameState*>*> trainAndTest = getTrainingSet(loadedStates, trainingSize);
	vector<GameState*>* trainingSet = trainAndTest.first;
	vector<GameState*>* testingSet = trainAndTest.second;

	cout << "Test and training sets established." << endl;

	vector<pair<int, vector<double> > > localMaxima;

	for (int runthrough = 0; runthrough < iterations; runthrough++)
	{
		//randomly insert a 0 - 1 floating point number for each of the variables
		for (size_t count = 0; count < 60; count++)
			variableScores[count] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

		//initialize the scores with the new variable scores
		cout << "initial correct: " << scoreGames(trainingSet).first << endl;

		double variation = 0.6;

		//changes within this loop are only affecting a couple games at a time, that doesn't seem right
		while (variation > 0.001)
		{
			//Score games and get the base value correct
			//ensures that all minutes are used and the sign is 1 so the previous score is subtracted, and the new score is added - canceling out for this case
			int correct = scoreGames(trainingSet, 0).first;
			int highestCorrect = correct;
			vector<pair<int, char> > scores;

			//	for each parameter, test the positive and negative variation and get the number of states it correctly classified
			for (unsigned int param = 0; param < 60; param++)
			{
				//testing postive and negative changes on all variables
				for (unsigned int positive = 0; positive < 2; positive++)
				{
					//keep the old score so we can calculate the new score quickly
					double prevScore = variableScores[param];

					//	if the change is positive use variation, if it is negative use -varation
					variableScores[param] += (positive == 1) ? variation : -variation;
					if (variableScores[param] > 1)
						variableScores[param] = 1;
					else if (variableScores[param] < 0)
						variableScores[param] = 0;

					//send in the index of the parameter being changed, the previous value of the score, and false to make sure the changes are not saved
					int updatedCorrect = scoreGames(trainingSet, 0, param * 2 + positive, prevScore, false).first;

					//record the number of correctly guessed states
					scores.push_back(make_pair(updatedCorrect, param * 2 + positive));
					//revert the variable to the previous value
					variableScores[param] = prevScore;
				}
			}

			//sort in decreasing order
			stable_sort(scores.rbegin(), scores.rend());

			bool updated = false;

			//this loop is not really necessary however I could add an input parameter to control how fast I converge on a local maximum

			for (size_t countChange = 0; countChange < 1; countChange++)
			{
				if (scores[countChange].first > highestCorrect)
				{
					updated = true;
					char paramFull = scores[countChange].second;
					char param = paramFull / 2;

					double prevScore = variableScores[param];

					variableScores[param] += variation*((paramFull % 2 == 1) ? 1 : -1);

					if (variableScores[param] > 1)
						variableScores[param] = 1;
					else if (variableScores[param] < 0)
						variableScores[param] = 0;

					scoreGames(trainingSet, 0, paramFull, prevScore, true);
					cout << int(param) << " changed to " << variableScores[param] << " from " << prevScore << " increased to " << scores[countChange].first << endl;
				}
			}

			//every time there is no variable that will increase the score, decrease the weight value and then try again
			if (!updated)
				variation = double(variation * 0.95);

		}

		int simulatedResult = scoreGames(trainingSet).first;
		int realResult = scoreGames(testingSet).first;

		std::cout << "simulated game states correct = " << simulatedResult << " Out of a total of " << trainingSet->size() << " states" << endl;
		std::cout << "real life game states correct = " << realResult << " Out of a total of " << testingSet->size() << " states" << endl;

		//save results of each of the variables into the list of local maxima
		localMaxima.push_back(make_pair(realResult, variableScores));
	}

	if (localMaxima.size() == 0)
		return -1;

	//sort in decreasing order
	sort(localMaxima.rbegin(), localMaxima.rend());


	ofstream out(std::to_string(league) + "_output.csv");
	out << "unit,avg,stdev,min,max,,1,2,3,4,5,6,7,8,9,10" << endl;

	ofstream best(std::to_string(league) + "_bestLearned.csv");
	best << "These weights gave the best results for league " << league << endl;

	//for each variable print out the name along with the min, max, std, avg values for the top 10 results
	for (size_t count = 0; count < 60; count++)
	{
		string line = "";

		if (count < 36 || count > 39)
		{
			switch (count + 1)
			{
			case 1: line = "probe	,"; break;
			case 2: line = "zealot	,"; break;
			case 3: line = "sentry	,"; break;
			case 4: line = "stalker	,"; break;
			case 5: line = "hightemplar	,"; break;
			case 6: line = "darktemplar	,"; break;
			case 7: line = "immortal	,"; break;
			case 8: line = "colossus	,"; break;
			case 9: line = "archon	,"; break;
			case 10: line = "observer	,"; break;
			case 11: line = "warpprism	,"; break;
			case 12: line = "phoenix	,"; break;
			case 13: line = "void ray	,"; break;
			case 14: line = "carrier	,"; break;
			case 15: line = "mothership	,"; break;
			case 16: line = "photon cannon	,"; break;
			case 17: line = "oracle	,"; break;
			case 18: line = "tempest	,"; break;
			case 19: line = "mothership core	,"; break;
			case 20: line = "interceptor	,"; break;
			case 21: line = "scv	,"; break;
			case 22: line = "marine	,"; break;
			case 23: line = "marauder	,"; break;
			case 24: line = "reaper	,"; break;
			case 25: line = "ghost	,"; break;
			case 26: line = "hellion	,"; break;
			case 27: line = "siegetank	,"; break;
			case 28: line = "thor	,"; break;
			case 29: line = "viking	,"; break;
			case 30: line = "medivac	,"; break;
			case 31: line = "banshee	,"; break;
			case 32: line = "raven	,"; break;
			case 33: line = "battlecruiser	,"; break;
			case 34: line = "planetaryfortress	,"; break;
			case 35: line = "missileturret	,"; break;
			case 36: line = "widowmine	,"; break;
			case 41: line = "drone	,"; break;
			case 42: line = "zergling	,"; break;
			case 43: line = "queen	,"; break;
			case 44: line = "baneling	,"; break;
			case 45: line = "roach	,"; break;
			case 46: line = "overlord	,"; break;
			case 47: line = "overseer	,"; break;
			case 48: line = "hydralisk	,"; break;
			case 49: line = "spinecrawler	,"; break;
			case 50: line = "sporecrawler	,"; break;
			case 51: line = "mutalisk	,"; break;
			case 52: line = "corruptor	,"; break;
			case 53: line = "broodlord	,"; break;
			case 54: line = "broodling	,"; break;
			case 55: line = "infestor	,"; break;
			case 56: line = "infestedterran	,"; break;
			case 57: line = "ultralisk	,"; break;
			case 58: line = "nydusworm	,"; break;
			case 59: line = "swarmhost	,"; break;
			case 60: line = "viper	,"; break;
			}

			best << line << localMaxima[0].second[count] << endl;

			double min = 1;
			double max = 0;
			double stdev = 0;
			double avg = 0;

			for (unsigned int top10 = 0; top10 < localMaxima.size() && top10 < 10; top10++)
			{
				if (localMaxima[top10].second[count] < min)
					min = localMaxima[top10].second[count];
				if (localMaxima[top10].second[count] > max)
					max = localMaxima[top10].second[count];

				avg += localMaxima[top10].second[count];
			}
			avg = avg / 10;

			//stdev
			double total = 0;
			for (int top10 = 0; top10 < 10; top10++)
			{
				total += pow(localMaxima[top10].second[count] - avg, 2);
			}
			stdev = sqrt(total / 10);

			//print 2 lines, details, then the whole list
			line += std::to_string(avg) + "," + std::to_string(stdev) + "," + std::to_string(min) + "," + std::to_string(max) + ",,";
			for (int top10 = 0; top10 < 10; top10++)
			{
				line += std::to_string(localMaxima[top10].second[count]) + ",";
			}
			std::cout << line << endl;
			out << line << endl;
		}
	}
	
	best.close();

	for (unsigned int top10 = 0; top10 < localMaxima.size() && top10 < 10; top10++){
		variableScores = localMaxima[top10].second;
		out << "Based on learned " << (top10 + 1) << ": " << scoreGames(testingSet).first << " out of " << testingSet->size() << endl;
	}

	out.close();


	//end of program get rid of the actual data
	for (int count = (int)loadedStates->size() - 1; count >= 0; count--)
	{
		delete (*loadedStates)[count];
	}
	delete loadedStates;

}

