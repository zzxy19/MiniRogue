//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Room.h
//


#ifndef ROOM_INCLUDED
#define ROOM_INCLUDED


#include "utilities.h"


class Room
{
public:
	// Constructor and destructor
	Room(int r, int c, int height, int width) : m_row(r), m_col(c), m_height(height), m_width(width) {}
	~Room() {}

	// Accessors
	int row() const { return m_row; }
	int col() const { return m_col; }
	int width() const { return m_width; }
	int height() const { return m_height; }
	void randPoint(int &r, int &c) const
	{
		r = m_row + randInt(m_height);
		c = m_col + randInt(m_width);
	}

private:
	int m_row; // row number of the upperleft corner
	int m_col; // column number of the upperleft corner
	int m_height;
	int m_width;
};


#endif // ROOM_INCLUDED