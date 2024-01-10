#ifndef _MENU_H
#define _MENU_H

#include "game.h"
#include "lpc17xx.h"
#include "directions.h"

/*
	MENU_choose
	---------------------------------------------------------------------
	Allows the user to choose an option of a menu
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the option choosen (values other than DIR_up and DIR_down
			are ignored)
*/
void MENU_choose(directions dir);


/*
	MENU_confirmChoice
	---------------------------------------------------------------------
	Confirms the current menu choice
*/
void MENU_confirmChoice(void);


/*
	MENU_boardChoiceMenu
	---------------------------------------------------------------------
	Generates the board choice menu
*/
void MENU_boardChoiceMenu(void);


/*
	MENU_playerTypeMenu
	---------------------------------------------------------------------
	Generates the player type choice menu
*/
void MENU_playerTypeMenu(void);

#endif
