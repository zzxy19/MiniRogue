//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Actor.h
//


#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED


#include "Weapon.h"
#include <string>


class Dungeon;

class Actor {
public:
	// Constructor and destructor
	Actor(int strength, int dex, int armor, int HP, int r, int c, std::string name, Weapon* weapon, Dungeon* dungeon);
	virtual ~Actor();


	// Accessors
	int strength() const;
	int dex() const;
	int armor() const;
	int hp() const;
	int sleep() const;
		// Return the sleepTime of the actor.
		// A sleepTime of 0 indicates actor is active; a sleepTime nonzero indicates actor is asleep.
	int row() const;
	int col() const;
	std::string name() const;
	Weapon* weapon() const;
	Dungeon* dungeon() const;
	virtual char icon() const = 0;
		// Different actors will return a char representing their individual icon.
		// For example, the player will return '@', whereas a snakewoman will return 'S'.
	bool isDead() const;
		// Return true if the actor is dead (hitPoint =< 0), and false otherwise.
	virtual bool isSmart() const = 0;
	virtual int smellDistance() const = 0;


	// Mutators
	void addHP(int amt);
	void fallAsleep(int time);
		// Take in time, and updates the actor's sleepTime to the maximum of time and current sleepTime.
	void wake();
	void move(int r, int c);
	

	// Mutators that only Player needs.
	virtual void cheat();
	virtual void addMaxHP(int amt);
	virtual void addStrength(int amt);
	virtual void addDex(int amt);
	virtual void addArmor(int amt);
	virtual void changeWeapon(Weapon* nWeapon);
	void changeDungeon(Dungeon* newDungeon);


	// Playing the game
	virtual std::string attack(Actor* victim);
		// attack() takes in a pointer to the actor under attack;
		// it then does all relevant math and automatically updates the status of the actor under attack.
		// If the actor under attack is killed, attack() returns true; otherwise, attack() returns false.
	virtual void selfheal();
		// For most actors, this function does nothing.
		// For player and dragon, calling this function will have a 10% chance to regain 1 hit point.

private:
	int m_strength;
	int m_dexterity;
	int m_armor;
	int m_hitPoint;
	int m_maxHP;
	int m_sleepTime;
	int m_row;
	int m_col;
	std::string m_name;
	Weapon* m_weapon;
	Dungeon* m_dungeon;
};


#endif // ACTOR_INCLUDED
