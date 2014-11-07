#ifndef LOADDATA_H
#define LOADDATA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <sstream>

#include "GameState.h"

vector<GameState*>* loadData(string fileName, int GivenLeague)
{
	vector<GameState*>* returning = new vector<GameState*>();

	ifstream myfile(fileName.c_str());
	string line;
	if (myfile.is_open())
	{
		//skip the header line
		getline(myfile, line);
		getline(myfile, line);
		bool nextLineIsNewGame = (line.find(",") > 0);

		while (nextLineIsNewGame)
		{
			//line is the current game information
			//grab the necessary game information - races and who won

			//skip gameID
			line = line.substr(line.find(",") + 1);
			//skip EntityIDP1
			line = line.substr(line.find(",") + 1);
			//skip EntityIDP2
			line = line.substr(line.find(",") + 1);

			//grab the race for the first player
			string P1Race = line.substr(0, line.find(","));
			line = line.substr(line.find(",") + 1);
			//grab the race for the second player
			string P2Race = line.substr(0, line.find(","));
			line = line.substr(line.find(",") + 1);
			string winnerString = line.substr(0, line.find(","));
			line = line.substr(line.find(",") + 1);
			int league = atoi(line.c_str());
			// p = 0-19
			// t = 20-39
			// z = 40-59

			int raceOffsetP1 = 0;
			if (P1Race == "T")
			{
				raceOffsetP1 = 20;
			}
			else if (P1Race == "Z")
			{
				raceOffsetP1 = 40;
			}
			int raceOffsetP2 = 0;
			if (P2Race == "T")
			{
				raceOffsetP2 = 20;
			}
			else if (P2Race == "Z")
			{
				raceOffsetP2 = 40;
			}

			bool winner = (winnerString == "P1");

			//grab the first gamestate
			if (!getline(myfile, line))
				nextLineIsNewGame = false;

			std::string line2 = "";
			nextLineIsNewGame = false;
			//if the current line is a gamestate
			while (line.find(",") == 0)
			{
				//grab the next line
				getline(myfile, line2);
				if (line2.find(",") != 0)
					nextLineIsNewGame = true;

				//this line is used to filter out all but the specified league, unless -1 was entered, then use all
				if (GivenLeague == -1 || league == GivenLeague)
				{
					GameState* temp = new GameState();
					temp->vUnitsP1 = new vector<pair<char, int>>();
					temp->vUnitsP2 = new vector<pair<char, int>>();
					temp->vUnitsP1->resize(60);
					temp->vUnitsP2->resize(60);
					temp->winner = winner;
					temp->playerOffset1 = raceOffsetP1;
					temp->playerOffset2 = raceOffsetP2;
					//read in all the values
					//skip blank comma 
					line = line.substr(line.find(",") + 1);

					temp->minute = atoi(line.substr(0, line.find(",")).c_str());
					line = line.substr(line.find(",") + 1);
					//skip P1
					line = line.substr(line.find(",") + 1);

					for (char countP1 = 0; countP1 < raceOffsetP1; countP1++)
					{
						(*temp->vUnitsP1)[countP1] = make_pair(countP1, 0);
					}
					for (char countP1 = 0; countP1 < 20; countP1++)
					{
						//insert the unit information for each variable					
						(*temp->vUnitsP1)[raceOffsetP1 + countP1] = make_pair(raceOffsetP1 + countP1, atoi(line.substr(0, line.find(",")).c_str()));

						line = line.substr(line.find(",") + 1);
//pulling this out from the calculateScores function so it can be used in multiple programs means I can't get the score initially
//						temp->score1 += vectorFindB(temp->vUnitsP1, raceOffsetP1 + countP1) * variableScores[raceOffsetP1 + countP1];
					}
					for (char countP1 = raceOffsetP1 + 20; countP1 < 60; countP1++)
					{
						(*temp->vUnitsP1)[countP1] = make_pair(countP1, 0);
					}

					//skip P2
					line = line.substr(line.find(",") + 1);

					for (char countP2 = 0; countP2 < raceOffsetP2; countP2++)
					{
						(*temp->vUnitsP2)[countP2] = make_pair(countP2, 0);
					}
					for (int countP2 = 0; countP2 < 20; countP2++)
					{
						//insert the unit information for each variable					
						//						temp->vUnitsP2->push_back(make_pair(raceOffsetP2 + countP2,  atoi(line.substr(0, line.find(",")).c_str())));
						(*temp->vUnitsP2)[raceOffsetP2 + countP2] = make_pair(raceOffsetP2 + countP2, atoi(line.substr(0, line.find(",")).c_str()));

						if (countP2 < 19)
						{	//don't grab the next section if this is the end of the current line
							line = line.substr(line.find(",") + 1);
						}
//pulling this out from the calculateScores function so it can be used in multiple programs means I can't get the score initially
//						temp->score2 += vectorFindB(temp->vUnitsP2, raceOffsetP2 + countP2) * variableScores[raceOffsetP2 + countP2];
					}
					for (char countP2 = raceOffsetP2 + 20; countP2 < 60; countP2++)
					{
						(*temp->vUnitsP2)[countP2] = make_pair(countP2, 0);
					}

					returning->push_back(temp);
				}
				line = line2;

			} //end of line

		} //end next line has a new game
		myfile.close();
	}//file was opened

	return returning;
}

#endif