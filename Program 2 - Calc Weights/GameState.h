#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
using namespace std;

class GameState
{
public:
	char minute;

	vector<pair<char, int> >* vUnitsP1;
	vector<pair<char, int> >* vUnitsP2;
	bool winner;
	double diff;

	int playerOffset1;
	int playerOffset2;

	double score1;
	double score2;
};

#endif