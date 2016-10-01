//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Game.h
//

#ifndef GAME_INCLUDED
#define GAME_INCLUDED


#include "Dungeon.h"
#include "Player.h"


class Game
{
public:
	Game(int goblinSmellDistance);
	~Game();
	void play();
	void createNewDungeon(int level);
	

private:
	int m_smellDistance;
	Dungeon* m_dungeon;
	Player* m_player;
};

#endif // GAME_INCLUDED
