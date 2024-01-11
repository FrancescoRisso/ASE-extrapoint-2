#ifndef _AI_H
#define _AI_H

#include "../game.h"
#include "lpc17xx.h"


#define randomIterations 7

extern int lastMove;

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


/*
	AI_move
	---------------------------------------------------------------------
	Possibly executes the AI's move
	Possibily in the sense that the AI randomly waits some seconds before
	doing the move, to be more similar to a player.
	---------------------------------------------------------------------
	PARAMETERS:
		- timeLeft: the number of seconds left before the time runs out
*/
void AI_move(int timeLeft);

#endif
