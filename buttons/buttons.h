#ifndef __BUTTON_H
#define __BUTTON_H

#include "../Game/game.h"
#include "../utils/NVIC.h"
#include "../utils/bool.h"
#include "../utils/pinConnections.h"
#include "lpc17xx.h"


/** if defined, the buttons are debounced with the help of RIT */
#define _BUTTONS_DEBOUNCE

/**
 * if defined, the RIT must be used exclusively for buttons.
 * This flag can prevent polling when not needed.
 * if not defined, RIT timer must be started by the user.
 */
//#define _BUTTONS_ENABLE_RIT_ON_PRESS

#ifdef _BUTTONS_DEBOUNCE
#ifndef _BUTTONS_ENABLE_RIT_ON_PRESS
#define _BUTTONS_ALLOW_ENABLE
#endif
#endif


typedef enum { BTN_int0, BTN_key1, BTN_key2 } button;
typedef enum { BTN_level, BTN_edge } BTN_sensitivity;
typedef enum { BTN_release, BTN_press, BTN_longPress } BTN_status;


#ifndef _BUTTONS_ALLOW_ENABLE
/*
	BTN_enable
	---------------------------------------------------------------------
	Enables a button and eventually its interrupt, and sets a priority
	---------------------------------------------------------------------
	PARAMETERS:
		- btn: the button
		- priority: the priority to be set.
		- sensitivity: if the button should be level- or edge-sensitive
		- enableInterrupt: whether the interrupt for the button should be
			enabled
*/
void BTN_enable(button btn, unsigned int priority, BTN_sensitivity sensitivity, bool enableInterrupt);
#endif


#ifdef _BUTTONS_DEBOUNCE
/*
	BTN_updateStatus
	---------------------------------------------------------------------
	Polls the button to know if it pressed or not, updates and returns
	the status
	Ideal to be used in a RIT to implement de-bouncing
	If _BUTTONS_DEBOUNCE is set, calls debounced handlers on first press
	---------------------------------------------------------------------
	PARAMETERS:
		- btn: the button to be read
	---------------------------------------------------------------------
	OUTPUT:
		- the current status of the button
*/
BTN_status BTN_updateStatus(button btn);

void BTN_updateStatusAll(void);
#endif

#endif
