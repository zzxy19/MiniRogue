//
// CS32 Project3
// MiniRogue
// Name: Zihan Xie
// ID: 704285618
// helper.h
// This file contains the declaration of some helper functions
//


#include "helper.h"


int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}


int min(int a, int b)
{
	if (a > b)
		return b;
	else
		return a;
}


int absolute(int a)
{
	if (a < 0)
		a = -a;
	return a;
}
