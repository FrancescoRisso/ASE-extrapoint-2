#include "menu.h"

char choiceOptions[2][30];
directions choosenOption = DIR_none;

extern gameStatuses gameStatus;
extern bool dualBoard;
extern int myID;

/*
	MENU_draw
	---------------------------------------------------------------------
	Displays a menu with two choices
	---------------------------------------------------------------------
	PARAMETERS:
		- row1: the first row of the prompt
		- row2: the second row of the prompt
		- opt1: the first option
		- opt2: the second option
	---------------------------------------------------------------------
	OUTPUT:
		-
*/
void MENU_draw(char *row1, char *row2);


/*
	MENU_drawOption
	---------------------------------------------------------------------
	Draws an option of the choice menu
	---------------------------------------------------------------------
	PARAMETERS:
		- optionNo: 0 or 1, if it is the first or second option
		- selected: if it is currently selected
*/
void MENU_drawOption(int optionNo, bool selected);


void MENU_draw(char *row1, char *row2) {
	LCD_Clear(backgroundColor);

	disable_RIT();

	GUI_Text((240 - 8 * strlen(row1)) / 2, 110, (uint8_t *) row1, textColor, backgroundColor);
	GUI_Text((240 - 8 * strlen(row2)) / 2, 130, (uint8_t *) row2, textColor, backgroundColor);

	MENU_drawOption(0, false);
	MENU_drawOption(1, false);

	enable_RIT();
}


void MENU_drawOption(int optionNo, bool selected) {
	int top = optionNo == 0 ? 160 : 200;
	int left = 40;
	int color = selected ? selectedMenuColor : backgroundColor;
	char *text = choiceOptions[optionNo];

	LCD_drawRect(left, 240 - left, top, top + 30, textColor, color);
	GUI_Text((240 - 8 * strlen(text)) / 2, top + 10, (uint8_t *) text, textColor, color);
}


void MENU_choose(directions dir) {
	disable_RIT();

	if(dir == DIR_up) {
		choosenOption = DIR_up;
		MENU_drawOption(0, true);
		MENU_drawOption(1, false);
	} else if(dir == DIR_down) {
		choosenOption = DIR_down;
		MENU_drawOption(0, false);
		MENU_drawOption(1, true);
	}

	enable_RIT();
}


void MENU_confirmChoice() {
	if(choosenOption == DIR_none) return;

	switch(gameStatus) {
		case GAME_chooseNumBoards:
			if(choosenOption == DIR_down) {
				GAME_twoBoardGame(true);
				myID = 0;
			} else {
				GAME_oneBoardGame();
				gameStatus = GAME_AIorPlayer;
			}
			break;

		case GAME_AIorPlayer: GAME_setPlayerType((bool) (choosenOption == DIR_up)); break;

		default: break;
	}

	choosenOption = DIR_none;
}


void MENU_boardChoiceMenu() {
	char opt1[] = "1 board\0";
	char opt2[] = "2 boards\0";

	memcpy(choiceOptions[0], opt1, 1 + strlen(opt1));
	memcpy(choiceOptions[1], opt2, 1 + strlen(opt2));

	MENU_draw("Select the", "GAME MODE");
}


void MENU_playerTypeMenu() {
	char opt1[] = "Human\0";
	char opt2[] = "NPC\0";

	memcpy(choiceOptions[0], opt1, 1 + strlen(opt1));
	memcpy(choiceOptions[1], opt2, 1 + strlen(opt2));

	if(dualBoard)
		MENU_draw("2 boards:", "select your player");
	else
		MENU_draw("1 board:", "select your enemy");
}
