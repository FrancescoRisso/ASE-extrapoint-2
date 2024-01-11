#ifndef _DIRECTIONS_H
#define _DIRECTIONS_H

#include "../utils/bool.h"
#include "lpc17xx.h"

typedef enum { DIR_up, DIR_down, DIR_left, DIR_right, DIR_up_right, DIR_up_left, DIR_down_right, DIR_down_left, DIR_none } directions;


/*
	DIR_sum
	---------------------------------------------------------------------
	Computes the sum of two directions, if possible (examples below)
	If it is not possible, returns DIR_none
	Examples:
	- up + down = none
	- none + up = up
	- up + left = up_left
	- up_left + right = up
	- up + none = up
	- up_left + down_right = none
	- up_left + left = none (outside the "3x3 grid")
	---------------------------------------------------------------------
	PARAMETERS:
		- dir1: the first base direction
		- dir2: the second base direction
	---------------------------------------------------------------------
	OUTPUT:
		- the diagonal (dir1 + dir2)
*/
directions DIR_sum(directions dir1, directions dir2);


/*
	DIR_getPerpendicular
	---------------------------------------------------------------------
	Returns one of the two directions opposite to the given one
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the original direction
		- primary: a way to distinguish the two equivalent directions
			primary order is right > up > left > down
	---------------------------------------------------------------------
	OUTPUT:
		- the opposite direction to dir, as chosen by primary
*/
directions DIR_getPerpendicular(directions dir, bool primary);


/*
	DIR_opposite
	---------------------------------------------------------------------
	Returns the opposite of a given direction (eg up <--> down)
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the given direction
	---------------------------------------------------------------------
	OUTPUT:
		- its opposite (with DIR_none <--> DIR_none)
*/
directions DIR_opposite(directions dir);

#endif
