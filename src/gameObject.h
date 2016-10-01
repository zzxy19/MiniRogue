//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// gameObject.h
// This file contains both the declaration and the implementation of the following class:
// gameObject
//


#ifndef GAMEOBJECT_INCLUDED
#define GAMEOBJECT_INCLUDED


#include <string>


class gameObject // gameObject is an abstract class
{
public: 
	// Constructor and Destructor
	gameObject(int row, int col) : m_row(row), m_col(col) {}
	virtual ~gameObject() = 0;
	
	// Accessors
	int row() { return m_row; }
	int col() { return m_col; }
	virtual std::string name() const = 0;
	virtual char icon() const = 0;

	// Playing the game
	void moveToInventory()
	{
		m_row = -1;
		m_col = -1;
	}

private:
	// A nonnegative m_col indicates a gameObject in the dungeon, where
	// m_row and m_col indicates the position of the object.
	// Negative m_col and m_row (typically -1) indicates a gameObject on the inventory.
	int m_row;
	int m_col;
};


inline gameObject::~gameObject() {}
// Destructor declared as pure virtual in declaration and implemented here only so that gameObject will be abstract



#endif // GAMEOBJECT_INCLUDED