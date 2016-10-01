//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Player.cpp
//


#include "Player.h"
#include <cstdlib>
#include <iostream>
using namespace std;


Player::Player(int r, int c, Dungeon* dungeon)
	: Actor(2, 2, 2, 20, r, c, "Player", new S_sword(-1, -1), dungeon), m_sizeInvent(0), m_capacityInvent(26)
{
	m_inventory[0] = weapon();
	m_sizeInvent++;
}


Player::~Player()
{
	for (int i = 0; i < m_sizeInvent; i++)
	{
		delete m_inventory[i];
	}
}


//////////////////////////////////
// Accessors
//////////////////////////////////
gameObject** Player::inventory()
{
	return m_inventory;
}


int Player::sizeOfInventory() const
{
	return m_sizeInvent;
}


char Player::icon() const
{
	return '@';
}


bool Player::isSmart() const
{
	return false;
}


int Player::smellDistance() const
{
	return 0;
}


void Player::displayInventory() const
{
	cout << "Inventory:" << endl;
	for (int i = 0; i < m_sizeInvent; i++)
		cout << (char)(i + 97) << ". " << m_inventory[i]->name() << endl;
}


//////////////////////////////////
// Mutators
//////////////////////////////////
bool Player::addToInventory(gameObject* item)
{
	if (item == nullptr || m_sizeInvent < 0)
		exit(2);
	if (m_sizeInvent >= m_capacityInvent)
		return false;
	m_inventory[m_sizeInvent] = item;
	m_sizeInvent++;
	return true;
}


string Player::wieldWeapon(int pos)
{
	if (pos < 0 || pos >= m_capacityInvent)
		return "";
	if (pos >= m_sizeInvent)
		return "";
	string result;
	// check if the object is indeed a weapon
	gameObject* newItem = m_inventory[pos];
	Weapon* newWeapon = dynamic_cast<Weapon*>(newItem);
	// if not a weapon, print out error and returns false
	if (newWeapon == nullptr)
	{
		Scroll* wrongThingToWield = dynamic_cast<Scroll*>(newItem);
		result += "You can't wield " + wrongThingToWield->name() + "\n";
		return result;
	}
	// otherwise, it is a weapon; wield it, print out message, and return true
	changeWeapon(newWeapon);
	result += "You are wielding " + weapon()->name() + "\n";
	return result;
}


string Player::readScroll(int pos, bool &tele)
{
	if (pos < 0 || pos >= m_capacityInvent)
		return "";
	if (pos >= m_sizeInvent)
		return "";
	string result;
	// check if the item is indeed a scroll
	gameObject* newItem = m_inventory[pos];
	Scroll* newScroll = dynamic_cast<Scroll*>(newItem);
	// if not a scroll, print out error and return false
	if (newScroll == nullptr)
	{
		Weapon* wrongThingToRead = dynamic_cast<Weapon*>(newItem);
		result += "You can't read a " + wrongThingToRead->name() + "\n";
		return result;
	}
	// otherwise, it is a scroll; identify which scroll it is by checking scroll id
	// execute the corresponding effect of the scroll, update the inventory, and return true
	char scrollID = newScroll->id();
	switch (scrollID)
	{
	case 'A':
		addArmor(newScroll->amount());
		break;

	case 'D':
		addDex(newScroll->amount());
		break;

	case 'H':
		addMaxHP(newScroll->amount());
		break;

	case 'S':
		addStrength(newScroll->amount());
		break;

	case 'T':
		tele = true;
		break;

	default:
		exit(3);
	}
	// print out the effect message, and return true
	result += "You read the scroll called " + newScroll->name() + "\n";
	result += newScroll->effect() + "\n";
	// update the inventory, erase the scroll at position pos
	eraseInvent(pos);
	return result;
}


bool Player::eraseInvent(int pos)
{
	if (pos < 0 || pos >= m_sizeInvent)
		return false;
	delete m_inventory[pos];
	// Shift all inventory objects behind m_inventory[pos] one step advance.
	for (int i = pos; i < m_sizeInvent - 1; i++)
	{
		m_inventory[i] = m_inventory[i + 1];
	}
	m_sizeInvent--;
	return true;
}


void Player::selfheal()
{
	if (trueWithProbability(0.1))
		addHP(1);
}