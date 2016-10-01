//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// allMonster.h
//


#ifndef ALLMONSTER_INCLUDED
#define ALLMONSTER_INCLUDED


#include "Actor.h"
class Dungeon;


class Bogeyman : public Actor
{
public:
	Bogeyman(int r, int c, Dungeon* dungeon);
	virtual ~Bogeyman();
	virtual char icon() const;
	virtual bool isSmart() const;
	virtual int smellDistance() const;
};


class Snakewoman : public Actor
{
public:
	Snakewoman(int r, int c, Dungeon* dungeon);
	virtual ~Snakewoman();
	virtual char icon() const;
	virtual bool isSmart() const;
	virtual int smellDistance() const;
};


class Goblin : public Actor
{
public:
	Goblin(int r, int c, Dungeon* dungeon);
	virtual ~Goblin();
	virtual char icon() const;
	virtual bool isSmart() const;
	virtual int smellDistance() const;
};


class Dragon : public Actor
{
public:
	Dragon(int r, int c, Dungeon* dungeon);
	virtual ~Dragon();
	virtual char icon() const;
	virtual bool isSmart() const;
	virtual int smellDistance() const;
	virtual void Dragon::selfheal();
};


#endif // ALLMONSTER_INCLUDED