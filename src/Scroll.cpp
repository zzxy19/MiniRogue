//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Scroll.cpp
// 


#include "Scroll.h"
#include "utilities.h"
#include <cstdlib>


//////////////////////////////////
// Constructor and destructor
//////////////////////////////////
Scroll::Scroll(int r, int c, char id)
	: gameObject(r, c), m_id(id)
{
	switch (id)
	{
	case 'A':
		m_name = "scroll of enhance armor";
		m_effect = "Your armor glows blue.";
		m_amount = randInt(3) + 1;
		break;

	case 'D':
		m_name = "scroll of enhance dexterity";
		m_effect = "You feel like less of a klutz.";
		m_amount = 1;
		break;

	case 'H':
		m_name = "scroll of enhance health";
		m_effect = "You feel your heart beating stronger.";
		m_amount = randInt(6) + 3;
		break;

	case 'S':
		m_name = "scroll of strength";
		m_effect = "Your muscles bulge.";
		m_amount = randInt(3) + 1;
		break;

	case 'T':
		m_name = "scroll of teleportation";
		m_effect = "You feel your body wrenched in space and time.";
		m_amount = 0;
		break;

	default:
		exit(1);
	}
}


Scroll::~Scroll()
{}


//////////////////////////////////
// Accessors
//////////////////////////////////
char Scroll::id()
{
	return m_id;
}


std::string Scroll::name() const
{
	return m_name;
}


std::string Scroll::effect()
{
	return m_effect;
}


int Scroll::amount()
{
	return m_amount;
}


char Scroll::icon() const
{
	return '?';
}