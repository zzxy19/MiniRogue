//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Game.cpp
//

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;


Game::Game(int goblinSmellDistance)
{
	m_smellDistance = goblinSmellDistance;
	m_dungeon = new Dungeon(0);
	// Initialize the dungeon.
	m_dungeon->initializeDungeon();
	// Randomly create some number of objects.
	m_dungeon->createObject();
	// Randomly create some number of monsters.
	m_dungeon->createMonster();
	// Randomly place the player.
	m_dungeon->createPlayer();
	// Let m_player points to the player in the dungeon.
	m_player = m_dungeon->player();
	// Update the m_maze[18][70] in Dungeon to reflect the current dungeon layout.
	m_dungeon->updateMaze();
	m_dungeon->displayMaze();
	m_dungeon->displayStatus();
}


Game::~Game()
{
	delete m_dungeon;
	delete m_player;
}


void Game::createNewDungeon(int level)
{
	clearScreen();
	// Delete the current dungeon, and create a new one with the new level.
	delete m_dungeon;
	m_dungeon = new Dungeon(level);
	// Initialize the new dungeon the same way as initially, but without creating a new Player.
	m_dungeon->initializeDungeon();
	m_dungeon->createObject();
	m_dungeon->createMonster();
	// Link the m_player with the new dungeon, and set the player in the new dungeon to be m_player.
	m_player->changeDungeon(m_dungeon);
	m_dungeon->setPlayer(m_player);
	// Generate a random coordinate that is valid in the new dungeon to place the player.
	int r, c;
	do
	{
		// generate a random valid coordinate (r,c) in a random room
		int index = randInt(m_dungeon->nRoom());
		m_dungeon->room()[index]->randPoint(r, c);
	} while (!m_dungeon->isMovable(r, c));
	m_player->move(r, c);
	// Update m_maze[18][70] to reflect the current layout of the dungeon.
	m_dungeon->updateMaze();
	m_dungeon->displayMaze();
	m_dungeon->displayStatus();
}


void Game::play()
{
	char input = getCharacter();
	bool success = false;
	// Continue asking user for input as long as player is not dead, user has not won, and input is not 'q'
	while (input != 'q' && !success && !m_player->isDead())
	{ 
		string pMessage; // message about the Player
		string mMessage; // messages about the monsters
		// If player is asleep, reduce its sleepiness by 1 and skip the player's round.
		if (m_player->sleep())
		{
			m_player->wake();
		}
		else // Player's round.
		{	
			// Determine what kind of command the user is inputting, and call the corresponding member function of Dungeon to do the job.
			switch (input)
			{
			case 'h':
			case 'j':
			case 'k':
			case 'l':
				pMessage = m_dungeon->direction_command(input);
				break;

			case 'g':
				pMessage = m_dungeon->pickup_command(success); 
				// this function will change success to true if the item picked up is the golden idol.
				break;

			case 'i':
			case 'r':
			case 'w':
				pMessage = m_dungeon->inventory_command(input);
				break;

			case 'c':
				m_dungeon->cheat_command();
				break;

			case '>':
				int result = m_dungeon->downstair_command(); 
				// This function returns the current level if player is standing on the staircase.
				// Otherwise, this function returns -1.
				if (result > -1)
				{
					createNewDungeon(result + 1);
					input = getCharacter();
					continue;
				}
				break;
			}
		}
		// After player's turn, call selfheal(), which restore player's HP 1 time out of 10.
		m_player->selfheal();
		m_dungeon->updateMaze();
		// Call dungeon->monster_move, which automatically perform the appropriate action for each monster.
		// Collect the message to be printed later.
		mMessage = m_dungeon->monster_move(m_smellDistance);
		clearScreen();
		m_dungeon->updateMaze();
		m_dungeon->displayMaze();
		m_dungeon->displayStatus();
		if (pMessage != "")
			cout << pMessage;
		if (mMessage != "")
			cout << mMessage;
		if (!success)
			input = getCharacter();
	}
	if (input != 'q')
	{
		cout << "Press q to exit game." << endl;
		getCharacter();
	}
}
