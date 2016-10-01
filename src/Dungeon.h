//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Dungeon.h
//


#ifndef DUNGEON_INCLUDED
#define DUNGEON_INCLUDED


#include "Actor.h"
#include "Player.h"
#include "Room.h"
#include "Golden.h"
#include <string>


class Dungeon {
public:
	// Constructor and destructor
	Dungeon(int level); 
		// NOTE: dungeon constructor does NOT create new Actors or new Objects.
		// It only creates the array that holds the pointers to such new objects.
		// Pointers in the member arrays are left unitialized until functions such as initializeDungeon, createObject, createMonster, are called to give values to them.

	~Dungeon(); 
		// NOTE: dungeon destructor does NOT delete Player. Player is left for ~Game to delete.
		// It deletes everything else except golden idol (which it only deletes if at level 4).
	

	// Accessors
	int level() const;
	bool isMovable(int r, int c) const;
	bool nextLevel(int r, int c) const;
	Player* player() const;
	Room** room() const;
	int nRoom() const;


	// Game command executer
	void setPlayer(Player* player);
	std::string monster_move(int distance);
	std::string direction_command(char direction);
	std::string pickup_command(bool &success);
		// Set success to true if the thing picked up is the golden idol.
	std::string inventory_command(char command);
	void cheat_command();
	void teleport_command();
	int downstair_command();
		// Return the current level if the Player is standing on the staircase.
		// Otherwise, downstair_command() returns -1.
	bool canAttack(int pos);
	bool dumbSmell(int pos);
	bool smartSmell(int sr, int sc, int er, int ec, int count, int &minimum, int &nextr, int &nextc);
	bool isNotMovable(int r, int c) const;


	// Dungeon populator
	bool initializeDungeon();
	Room* createRoom(int r, int c, int h, int w);
		// This function creates a pseudo-random room based on the fed parameters.
	void makeConnectedRoom();
		// This function makes use of createRoom function and creates a series of pseudo-random rooms that are connected.
	bool createStair();
	bool createGolden();
	bool createObject();
	bool createMonster();
	bool createPlayer();
	

	// Monster and gameObject operators
	bool addMonster(int r, int c, char id);
	bool removeMonster(int r, int c);
	bool removeMonster(int pos);
	bool addObject(int r, int c, char id);
	bool removeObject(int r, int c);
		// NOTE: this function will not delete the object, but merely remove it from the m_object list!
		// Calling this function should be immediately followed by adding the object to player inventory to avoid memory leak.
	bool removeObject(int pos);
		// NOTE: this function will not delete the object, but merely remove it from the m_object list!
		// Calling this function should be immediately followed by adding the object to player inventory to avoid memory leak.
	bool dropObject(int r, int c, char icon);


	// Maze operators
	void updateMaze();
		// This function update m_maze[18][70] to reflect the most current layout of the dungeon, since m_maze might be outdated.
	void displayMaze() const;


	// Build-up operator
	void buildRoom(int index);
		// Update m_maze[18][70] to reflect m_room[i] by changing all characters within m_room[i] to " ".
	void buildWall();
		


	// display status
	void displayStatus() const;


private:
	char m_maze[18][70];
	Golden* m_golden;
		// the entire dungeon represented as a maze of characters
	Room** m_room;
	int m_nRoom;
	int m_maxRoom;
		// a dynamically allocated array of pointers to all rooms in the dungeon
	Actor** m_monster;
	int m_nMonster;
	int m_maxMonster;
		// a dynamically allocated array of pointers to all dynamically allocated monsters
	Player* m_player;
		// a pointer to the dynamically allocated player
	gameObject** m_object;
	int m_nObject;
	int m_maxObject;
		// a dynamically allocated array of pointers to all dynamically allocated objects
	int m_level;
	int m_stairR;
	int m_stairC;
};


#endif // DUNGEON_INCLUDED
