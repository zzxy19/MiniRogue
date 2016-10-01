//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Actor.cpp
//


#include "Actor.h"
#include "utilities.h"
#include "helper.h"
#include <iostream>
using namespace std;


//////////////////////////////////
// Construtor and destructor
//////////////////////////////////
Actor::Actor(int strength, int dex, int armor, int HP, int r, int c, string name, Weapon* weapon, Dungeon* dungeon)
	:m_strength(strength), m_dexterity(dex), m_armor(armor), m_hitPoint(HP), m_maxHP(HP), m_row(r), m_col(c), m_name(name), m_weapon(weapon), m_sleepTime(0), m_dungeon(dungeon)
{}


Actor::~Actor()
{}


//////////////////////////////////
// Accessors
//////////////////////////////////
int Actor::strength() const 
{
	return m_strength; 
}


int Actor::dex() const 
{
	return m_dexterity;
}


int Actor::armor() const 
{
	return m_armor;
}


int Actor::hp() const
{ 
	return m_hitPoint;
}


int Actor::sleep() const 
	// Return the sleepTime of the actor.
	// A sleepTime of 0 indicates actor is active; a sleepTime nonzero indicates actor is asleep.
{
	return m_sleepTime;
}


int Actor::row() const 
{
	return m_row;
}


int Actor::col() const 
{
	return m_col; 
}


string Actor::name() const 
{
	return m_name; 
}


Weapon* Actor::weapon() const
{
	return m_weapon; 
}


Dungeon* Actor::dungeon() const
{
	return m_dungeon;
}


bool Actor::isDead() const 
	// Return true if the actor is dead (hitPoint =< 0), and false otherwise.
{
	return m_hitPoint <= 0;
}


//////////////////////////////////
// Mutators
//////////////////////////////////
void Actor::addHP(int amt)
{
	m_hitPoint += amt;
	if (m_hitPoint > m_maxHP)
		m_hitPoint = m_maxHP;
}


void Actor::fallAsleep(int time)
	// Take in time, and updates the actor's sleepTime to the maximum of time and current sleepTime.
{
	m_sleepTime = max(time, m_sleepTime);
}


void Actor::wake()
{
	m_sleepTime--;
}


void Actor::move(int r, int c)
{
	m_row = r;
	m_col = c;
} 


//////////////////////////////////
// Mutators that only Player needs
//////////////////////////////////
void Actor::cheat()
{
	m_maxHP = 50;
	m_hitPoint = 50;
	m_strength = 9;
}


void Actor::addMaxHP(int amt)
{
	m_maxHP += amt;
	if (m_maxHP < 0)
		m_maxHP = 0;
	else if (m_maxHP > 99)
		m_maxHP = 99;
}


void Actor::addStrength(int amt)
{
	m_strength += amt;
	if (m_strength < 0)
		m_strength = 0;
	else if (m_strength > 99)
		m_strength = 99;
}


void Actor::addDex(int amt)
{
	m_dexterity += amt;
	if (m_dexterity < 0)
		m_dexterity = 0;
	else if (m_dexterity > 99)
		m_dexterity = 99;
}


void Actor::addArmor(int amt)
{
	m_armor += amt;
	if (m_armor < 0)
		m_armor = 0;
	else if (m_armor > 99)
		m_armor = 99;
}


void Actor::changeWeapon(Weapon* nWeapon)
{
	if (nWeapon != nullptr)
		m_weapon = nWeapon;
}


void Actor::changeDungeon(Dungeon* newDungeon)
{
	m_dungeon = newDungeon;
}


//////////////////////////////////
// Playing the game
//////////////////////////////////
string Actor::attack(Actor* victim)
	// attack() takes in a pointer to the actor under attack;
	// it then does all relevant math and automatically updates the status of the actor under attack.
	// It returns the string to be printed.
{
	string result;
	result += m_name + " " + m_weapon->verb() + " " + m_weapon->name() + " at " + victim->name() + " and ";
	int attackPoint = m_dexterity + m_weapon->dex();
	int defendPoint = victim->m_dexterity + victim->m_weapon->dex();

	// If miss, simply print out miss and return false.
	if (randInt(attackPoint) < randInt(defendPoint))
	{
		result += "misses.\n";
		return result;
	}

	// Otherwise, it is a hit. Do all subsequent math and updates the defender's status.
	result += "hits";
	int damagePoint = randInt(m_strength + m_weapon->damage());
	victim->addHP(-1 * damagePoint);

	// If victim is dead, print out death and return true.
	if (victim->isDead())
	{
		result += ", dealing a final blow.\n";
		return result;
	}

	// Otherwise, the victim is alive; check if victim is put to sleep, updates sleepTime of victim and return false.
	int sleep = m_weapon->putToSleep();
	if (sleep)
	{
		result += ", putting " + victim->name() + " to sleep";
		victim->fallAsleep(sleep);
	}
	result += ".\n";
	return result;
}


void Actor::selfheal()
	// For most actors, this function does nothing, as is implemented here.
	// Player and Dragon class will have a selfheal() that overrides this function.
	// For player and dragon, calling this selfheal() will have a 10% chance to regain 1 hit point.
{}