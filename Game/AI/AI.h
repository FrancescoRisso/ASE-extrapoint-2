#ifndef _AI_H
#define _AI_H

#include "lpc17xx.h"

#define randomIterations 7

/*
	AI_random
	---------------------------------------------------------------------
	Returns a random integer between 0 and max (included)
	---------------------------------------------------------------------
	PARAMETERS:
		- max: the maximum allowed value (included)
	---------------------------------------------------------------------
	OUTPUT:
		- a random number
*/
int AI_random(int max);

#endif
