/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "../Game/game.h"
#include "../buttons/buttons.h"
#include "../joystick/joystick.h"
#include "../utils/NVIC.h"
#include "../utils/bool.h"
#include "RIT.h"
#include "lpc17xx.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile bool joystickEnabled = true;
volatile bool int0Enabled = false;
volatile bool key1Enabled = false;
volatile bool key2Enabled = false;

extern gameStatuses gameStatus;

void RIT_IRQHandler(void) {
	int dir;

	if(joystickEnabled) {
		for(dir = 0; dir < 4; dir++)
			if(JOY_updateStatus((JOY_direction) dir) == JOY_press) switch(gameStatus) {
					case GAME_game: GAME_move((directions) dir); break;
					default: MENU_choose((directions) dir); break;
				}

		if(JOY_updateStatus(JOY_centerPress) == JOY_press) switch(gameStatus) {
				case GAME_chooseNumBoards: MENU_confirmChoice(); break;
				case GAME_game: GAME_endOfTurn(); break;
			}
	}

	if(int0Enabled) BTN_updateStatus(BTN_int0);
	if(key1Enabled) BTN_updateStatus(BTN_key1);
	if(key2Enabled) BTN_updateStatus(BTN_key2);

	myNVIC_clearInterrupt(myNVIC_rit); /* clear interrupt flag */
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
