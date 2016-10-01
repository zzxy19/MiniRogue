//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Weapon.h
// This file contains both the declaration and the implementation of the following class:
// Weapon, Mace, S_sword, L_sword, M_axe, M_fang
//


#ifndef WEAPON_INCLUDED
#define WEAPON_INCLUDED


#include <string>
#include "gameObject.h"


class Weapon : public gameObject // Weapon is an abstract class
{
public:
	// Constructor and destructor
	Weapon(int damage, int dex, std::string name, std::string verb, int row, int col)
		: m_damage(damage), m_dexBonus(dex), m_name(name), m_verb(verb), gameObject(row, col)
	{}
	virtual ~Weapon() {}

	// Accessors
	int damage() const { return m_damage; }
	int dex() const { return m_dexBonus; }
	virtual std::string name() const { return m_name; }
	std::string verb() const { return m_verb; }
	virtual char icon() const { return ')'; }

	// Pure virtual functions sleepTime
	virtual int putToSleep() const = 0;
		// For all weapons other than Magic fangs of sleep, function sleepTime should return 0.
		// For Magic fangs of sleep, the function sleepTime should return the number of moves that the attacker is put to sleep;
		// If Magic fangs of sleep (4 out of 5 times) fails to perform its magic, sleepTime returns 0.
	

private:
	int m_damage;
	int m_dexBonus;
	std::string m_name;
	std::string m_verb;
};


class Mace : public Weapon
{
public:
	// Constructor and destructor
	Mace(int row, int col) : Weapon(2, 0, "mace", "swings", row, col) {}
	virtual ~Mace() {}

	// sleepTime
	virtual int putToSleep() const { return 0; }
};


class S_sword : public Weapon
{
public:
	// Constructor and destructor
	S_sword(int row, int col) : Weapon(2, 0, "short sword", "slashes", row, col) {}
	virtual ~S_sword() {}

	// sleepTime
	virtual int putToSleep() const { return 0; }
};


class L_sword : public Weapon
{
public:
	// Constructor and destructor
	L_sword(int row, int col) : Weapon(4, 2, "long sword", "swings", row, col) {}
	virtual ~L_sword() {}

	// sleepTime
	virtual int putToSleep() const { return 0; }
};


class M_axe : public Weapon
{
public:
	// Constructor and destructor
	M_axe(int row, int col) : Weapon(5, 5, "magic axe", "chops", row, col) {}
	virtual ~M_axe() {}

	// sleepTime
	virtual int putToSleep() const { return 0; }
};


#include "utilities.h"

class M_fang : public Weapon
{
public:
	// Constructor and destructor
	M_fang(int row, int col) : Weapon(2, 3, "magic fangs of sleep", "strikes", row, col) {}
	virtual ~M_fang() {}

	// sleepTime
	virtual int putToSleep() const
	{
		if (trueWithProbability(0.2))
			return 2 + randInt(5);
		else
			return 0;
	}
};


#endif // WEAPON_INCLUDED
