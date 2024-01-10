#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "../utils/pinConnections.h"
#include "lpc17xx.h"


typedef enum { JOY_up, JOY_down, JOY_left, JOY_right, JOY_centerPress } JOY_direction;
typedef enum { JOY_release, JOY_press, JOY_longPress } JOY_status;


/*
	JOY_init
	---------------------------------------------------------------------
	Initializes all the registers needed for the joystick to work
*/
void JOY_init(void);


/*
	JOY_updateStatus
	---------------------------------------------------------------------
	Updates and returns the current status of the joystick in a given
	direction
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the requested direction
	---------------------------------------------------------------------
	OUTPUT:
		- if the joystick in that direction is released, pressed, or has
			been pressed for more than one update
*/
JOY_status JOY_updateStatus(JOY_direction dir);


#endif
