//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// Golden.h
//


#ifndef GOLDEN_INCLUDED
#define GOLDEN_INCLUDED


#include "gameObject.h"


class Golden : public gameObject
{
public:
	// Constructor and destructor
	Golden(int r, int c)
		: gameObject(r, c)
	{}
	virtual ~Golden() {}

	// Accessors
	virtual std::string name() const { return "the golden idol"; }
	virtual char icon() const { return '&'; }
};


#endif // GOLDEN_INCLUDED