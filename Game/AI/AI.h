#ifndef _AI_H
#define _AI_H

#include <stdlib.h>
#include <time.h>

#include "../game.h"
#include "lpc17xx.h"


// #define useFirstAlgorithm
#define useSecondAlgorithm


#define randomIterations 7


#define randomMoveWithPreviousValid 4

#define randomWeightOfPlayingLater 6

#define randomWeightOfReplacingEquallyGoodWall 2

#define randomWeightOfPlacingWall 3
#define randomWeightOfMovingToken 5

#define randomWeightOfCorrectDir 10
#define randomWeightOfWrongDir 2
#define randomWeightOfUselessDir 4

#define randomMaxWallWeight 8


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


/*
	AI_tryMirroringMove
	---------------------------------------------------------------------
	If the previous opponent's move was a valid one, tries to mirror it.
	There is a chance that it will not mirror a move, even if it was
	valid (in order to reduce predictability)
	This chance is (1 / (randomMoveWithPreviousValid + 1))
	---------------------------------------------------------------------
	OUTPUT:
		- if some move was done or not
*/
bool AI_tryMirroringMove(void);


/*
	AI_randomAction
	---------------------------------------------------------------------
	Performs a random action between placing a wall, moving the token and
	skipping the turn
*/
void AI_randomAction(void);


/*
	AI_moveToken
	---------------------------------------------------------------------
	Selects a valid move of the token and executes it
*/
void AI_moveToken(void);


/*
	AI_wallIsCorrect
	---------------------------------------------------------------------
	Checks if the current tmpWall can be inserted
	WARNING: it supposes the player still has walls available
	---------------------------------------------------------------------
	OUTPUT:
		- if the current tmpWall can be inserted
*/
bool AI_wallIsCorrect(void);


/*
	AI_placeWall
	---------------------------------------------------------------------
	Selects a valid wall position and places it
	WARNING: it supposes the player still has walls available
*/
void AI_placeWall(void);


#endif
