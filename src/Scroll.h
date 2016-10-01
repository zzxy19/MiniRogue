//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Scroll.h
// 


#ifndef SCROLL_INCLUDED
#define SCROLL_INCLUDED


#include <string>
#include "gameObject.h"


class Scroll : public gameObject
{
public:
	// Constructor and destructor
	Scroll(int r, int c, char id);
	virtual ~Scroll();
	
	// Accessors
	char id();
	virtual std::string name() const;
	std::string effect();
	int amount();
	virtual char icon() const;

private:
	std::string m_name;
	std::string m_effect;
	int m_amount;
	char m_id;
};


#endif // SCROLL_INCLUDED