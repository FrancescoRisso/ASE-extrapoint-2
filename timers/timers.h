#ifndef __TIMER_H
#define __TIMER_H

#include "../Game/game.h"
#include "../utils/NVIC.h"
#include "../utils/bool.h"
#include "../utils/power.h"
#include "lpc17xx.h"



typedef enum { TIMER_0, TIMER_1, TIMER_2, TIMER_3 } timer;

typedef enum {
	TIMER_nothing,
	TIMER_interrupt,
	TIMER_reset,
	TIMER_reset_interrupt,
	TIMER_stop,
	TIMER_stop_interrupt,
	TIMER_stop_reset,
	TIMER_stop_reset_interrupt
} timerCounterAction;

typedef enum { TIMER_matchReg0, TIMER_matchReg1, TIMER_matchReg2, TIMER_matchReg3 } matchRegister;

/*
	TIMER_enable
	---------------------------------------------------------------------
	Enables a timer, while also turning it on
	---------------------------------------------------------------------
	PARAMETERS:
		- t: the timer
*/
void TIMER_enable(timer t);


/*
	TIMER_disable
	---------------------------------------------------------------------
	Disables a timer, while also turning it off
	---------------------------------------------------------------------
	PARAMETERS:
		- t: the timer
*/
void TIMER_disable(timer t);


/*
	TIMER_enableReset
	---------------------------------------------------------------------
	Tells a timer if it should be resetting now
	---------------------------------------------------------------------
	PARAMETERS:
		- t: the timer
		- enable: a boolean telling if the timer should reset
*/
void TIMER_enableReset(timer t, bool enable);


/*
	TIMER_setValue
	---------------------------------------------------------------------
	Sets a value to a match register of a timer, selecting how the timer
	should behave when that value is reached
	---------------------------------------------------------------------
	PARAMETERS:
		- t: the timer
		- mr: which match register should be set
		- value: the value to be set
		- action: what to do when the counter is reached
*/
void TIMER_setValue(timer t, matchRegister mr, int value, timerCounterAction action);


#endif
