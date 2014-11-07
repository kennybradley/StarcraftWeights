//============================================================================
// Name        : SC-Project1.cpp
// Author      : Kenny Bradley
// Version     : 1
// Copyright   : 
// Description : This takes in 3 separate csvs and outputs the merged results into a single csv output
//============================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
using namespace std;

class MinuteData
{
public:
	int minute;
	string units;
};

class GameData
{
public:
	int entity_P1;
	int entity_P2;

	string Race1;
	string Race2;

	vector<MinuteData> gameMinutesP1;
	vector<MinuteData> gameMinutesP2;

	double league;
	bool P1wins;
};

int main() {
	//id minutes date league

	ifstream matchesFile("C:\\Users\\Kenny\\Desktop\\matches.csv");
	string line;


	map<int, int> ID_League;

	//entity id, minute, as?, u0,u1,....u19
	if (matchesFile.is_open())
	{
		int ID = 0;
		int League = 0;

		//skip the header
		getline(matchesFile, line);
		while (getline(matchesFile, line))
		{
			ID = atoi(line.substr(0, line.find(",")).c_str());
			//insert data into the Minute Data object
			line = line.substr(line.find(",") + 1);
			//skip duration
			line = line.substr(line.find(",") + 1);
			//skip date
			line = line.substr(line.find(",") + 1);
			League = atoi(line.substr(0, line.find(",")).c_str());

			ID_League[ID] = League;
		}
		matchesFile.close();
	}


	ifstream minutesFile("C:\\Users\\Kenny\\Desktop\\minutes.csv");
	line = "";

	unordered_map<string, vector<MinuteData> > minuteMap;

	//entity id, minute, as?, u0,u1,....u19
	if (minutesFile.is_open())
	{
		string entityID = "";
		//skip the header
		getline(minutesFile, line);
		while (getline(minutesFile, line))
		{
			MinuteData temp;
			entityID = line.substr(0, line.find(","));
			//insert data into the Minute Data object
			line = line.substr(line.find(",") + 1);
			temp.minute = atoi(line.substr(0, line.find(",")).c_str());

			//skip whatever the AS column is
			line = line.substr(line.find(",") + 1);
			line = line.substr(line.find(",") + 1);

			temp.units = line;
			minuteMap[entityID].push_back(temp);
		}
		minutesFile.close();
	}

	ifstream entsFile("C:\\Users\\Kenny\\Desktop\\ents.csv");
	unordered_map<string, GameData>  MergedGameData;
	line = "";
	string line2 = "";
	if (entsFile.is_open())
	{
		string matchID = "";
		string entityID = "";
		//skip header
		getline(entsFile, line);
		while (getline(entsFile, line))
		{
			if (!getline(entsFile, line2))
			{	//if there is not another line
				break;
			}
			
			matchID = line.substr(0, line.find(","));
			if (matchID != line2.substr(0, line2.find(",")))
			{
				cout << "Offset incorrect" << endl;
				break;
			}

			//		matchid, identity, entity_id, Race, win
			
			line = line.substr(line.find(",") + 1);
			line2 = line2.substr(line2.find(",") + 1);
			//identity skipped
			line = line.substr(line.find(",") + 1);
			line2 = line2.substr(line2.find(",") + 1);
			//insert minuteData
			string Player1EntityID = line.substr(0, line.find(","));
			string Player2EntityID = line2.substr(0, line2.find(","));
			line = line.substr(line.find(",") + 1);
			line2 = line2.substr(line2.find(",") + 1);

			string Player1Race = line.substr(0, line.find(","));
			string Player2Race = line2.substr(0, line2.find(","));
			line = line.substr(line.find(",") + 1);
			bool P1Win = false;
			if (line.substr(0, line.find(",")) == "true")
				P1Win = true;


			MergedGameData[matchID].entity_P1 = atoi(Player1EntityID.c_str());
			MergedGameData[matchID].entity_P2 = atoi(Player2EntityID.c_str());
			MergedGameData[matchID].gameMinutesP1 = minuteMap[Player1EntityID];
			MergedGameData[matchID].gameMinutesP2 = minuteMap[Player2EntityID];
			MergedGameData[matchID].Race1 = Player1Race;
			MergedGameData[matchID].Race2 = Player2Race;
			MergedGameData[matchID].P1wins = P1Win;
			MergedGameData[matchID].league = (ID_League[atoi(matchID.c_str())]);
		}
		entsFile.close();
	}
	
	//write out to a new file	
	ofstream outFile("C:\\Users\\Kenny\\Desktop\\UpdatedData.csv");
	outFile << "GameID,EntityIDP1,EntityIDP2,Race1,Race2,Winner,League" << endl;
	int numSkipped = 0;
	for (unordered_map<string, GameData>::iterator it = MergedGameData.begin(); it != MergedGameData.end(); it++)
	{
		if (it->second.gameMinutesP1.size() > 0)
		{
			string GameID = it->first;
			outFile << GameID << "," << it->second.entity_P1 << "," << it->second.entity_P2 << "," << it->second.Race1 << "," << it->second.Race2 << "," << (it->second.P1wins ? "P1" : "P2") << "," << it->second.league << endl;
			for (size_t count = 0; count < it->second.gameMinutesP1.size(); count++)
			{
				outFile << "," << it->second.gameMinutesP1[count].minute << ",P1," << it->second.gameMinutesP1[count].units << ",P2," << it->second.gameMinutesP2[count].units << endl;
			}
		}
		else
			cout << ++numSkipped << endl;
	}
	outFile.close();

	return 0;
}

