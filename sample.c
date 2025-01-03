/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through debounced buttons and Joystick
 *        	- key1 switches on the led at the left of the current led on,
 *					- it implements a circular led effect,
 * 					- joystick select function returns to initial configuration (led11 on) .
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "CAN/CAN.h"
#include "GLCD/GLCD.h"
#include "LPC17xx.H" /* LPC17xx definitions                */
#include "RIT/RIT.h"
#include "buttons/buttons.h"
#include "game/debugOpts.h"
#include "game/game.h"
#include "timers/timers.h"
#include "utils/power.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag;  // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h
						   // but since it is RO, it needs more work)
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main(void) {
	SystemInit(); /* System Initialization (i.e., PLL)  */
	LCD_Initialization();

	CAN_Init();

	init_RIT(TIME_50MS);
	enable_RIT();

#ifndef DEBUG_avoidRandom
	TIMER_enable(TIMER_2);
	TIMER_setValue(TIMER_2, TIMER_matchReg0, 1234567890, TIMER_reset);
#endif

#ifdef DEBUG_skipMenu
	GAME_init();
#else
	MENU_boardChoiceMenu();
#endif

	POW_setPowerMode(POW_sleep);

	while(1) { /* Loop forever                       */
		__ASM("wfi");
	}
}
