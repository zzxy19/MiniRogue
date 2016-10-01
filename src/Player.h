//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Player.h
//


#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED


#include "Actor.h"
#include "gameObject.h"
#include "Weapon.h"
#include "Scroll.h"
#include <string>


class Dungeon;

class Player : public Actor
{
public:
	// Constructor and destructor
	Player(int r, int c, Dungeon* dungeon);
	virtual ~Player();

	// Accessors
	gameObject** inventory();
	int sizeOfInventory() const;
	virtual char icon() const;
	virtual bool isSmart() const;
	virtual int smellDistance() const;

	// Mutators
	bool addToInventory(gameObject* item);
	std::string wieldWeapon(int pos);
	std::string readScroll(int pos, bool &tele);
	bool eraseInvent(int pos);
	void selfheal();

	// Playing the game
	void displayInventory() const;


private:
	gameObject* m_inventory[26];
	int m_sizeInvent;
	int m_capacityInvent;
};


#endif // PLAYER_INCLUDED