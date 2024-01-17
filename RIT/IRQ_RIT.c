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

directions lastDir = DIR_none;

void RIT_IRQHandler(void) {
	bool joyUp, joyDown, joyLeft, joyRight;
	directions dir;

	if(joystickEnabled) {
		joyUp = (bool) (JOY_updateStatus(JOY_up) != JOY_release);
		joyDown = (bool) (JOY_updateStatus(JOY_down) != JOY_release);
		joyLeft = (bool) (JOY_updateStatus(JOY_left) != JOY_release);
		joyRight = (bool) (JOY_updateStatus(JOY_right) != JOY_release);

		switch(gameStatus) {
			case GAME_chooseNumBoards:
			case GAME_AIorPlayer:
				if(joyUp) MENU_choose(DIR_up);
				if(joyDown) MENU_choose(DIR_down);
				break;
			case GAME_game:
				dir = DIR_none;
				if(joyUp) dir = DIR_sum(dir, DIR_up);
				if(joyDown) dir = DIR_sum(dir, DIR_down);
				if(joyLeft) dir = DIR_sum(dir, DIR_left);
				if(joyRight) dir = DIR_sum(dir, DIR_right);
				if(dir != DIR_none && dir != lastDir) GAME_move(dir);
				lastDir = dir;
				break;
		}

		if(JOY_updateStatus(JOY_centerPress) == JOY_press) switch(gameStatus) {
				case GAME_chooseNumBoards: MENU_confirmChoice(); break;
				case GAME_AIorPlayer: MENU_confirmChoice(); break;
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
