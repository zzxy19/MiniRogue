//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// allMonster.cpp
//


#include "allMonster.h"
#include "utilities.h"
using namespace std;


//////////////////////////////////
// Bogeyman
//////////////////////////////////
Bogeyman::Bogeyman(int r, int c, Dungeon* dungeon)
	: Actor(2 + randInt(2), 2 + randInt(2), 2, 5 + randInt(6), r, c, "the Bogeyman", new S_sword(-1, -1), dungeon)
{}


Bogeyman::~Bogeyman()
{}


char Bogeyman::icon() const
{
	return 'B';
}


bool Bogeyman::isSmart() const
{
	return false;
}


int Bogeyman::smellDistance() const
{
	return 5;
}


//////////////////////////////////
// Snakewoman
//////////////////////////////////
Snakewoman::Snakewoman(int r, int c, Dungeon* dungeon)
	: Actor(2, 3, 3, 3 + randInt(4), r, c, "the Snakewoman", new M_fang(-1, -1), dungeon)
{}


Snakewoman::~Snakewoman()
{}


char Snakewoman::icon() const
{
	return 'S';
}


bool Snakewoman::isSmart() const
{
	return false;
}


int Snakewoman::smellDistance() const
{
	return 3;
}



//////////////////////////////////
// Goblin
//////////////////////////////////
Goblin::Goblin(int r, int c, Dungeon* dungeon)
	: Actor(3, 1, 1, 15 + randInt(6), r, c, "the Goblin", new S_sword(-1, -1), dungeon)
{}


Goblin::~Goblin()
{}


char Goblin::icon() const
{
	return 'G';
}


bool Goblin::isSmart() const
{
	return true;
}


int Goblin::smellDistance() const
{
	return 16;
}


//////////////////////////////////
// Dragon
//////////////////////////////////
Dragon::Dragon(int r, int c, Dungeon* dungeon)
	: Actor(4, 4, 4, 20 + randInt(6), r, c, "the Dragon", new L_sword(-1, -1), dungeon)
{}


Dragon::~Dragon()
{}


char Dragon::icon() const
{
	return 'D';
}


void Dragon::selfheal()
{
	if (trueWithProbability(0.1))
		addHP(1);
}


bool Dragon::isSmart() const
{
	return false;
}


int Dragon::smellDistance() const
{
	return 0;
}
