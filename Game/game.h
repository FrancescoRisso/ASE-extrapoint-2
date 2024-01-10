#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <string.h>

#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"
#include "../timers/timers.h"
#include "../utils/abs.h"
#include "../utils/bool.h"
#include "lpc17xx.h"


#define gridSize 7

#define spaceSize 2
#define squareSize 32

#define numWalls 8;
#define totNumWalls 16;
// #define totNumWalls numWalls * 2;

#define gridColorVal 0xA0
#define gridColor RGB565CONVERT(gridColorVal, gridColorVal, gridColorVal)

#define fixedWallColor Black
// #define placingWallColor RGB565CONVERT(50, 161, 79)
#define placingWallColor Green

#define player1Color RGB565CONVERT(0xFF, 0, 0)
#define player2Color RGB565CONVERT(0, 0, 0xFF)
#define availableMoveColor RGB565CONVERT(0xf9, 0xf9, 0xc6)

#define backgroundColor White
#define textColor Black
#define selectedMenuColor Yellow


#define timerX 130
#define timerY 285
#define timerH 15
#define timerWperVal 5


#define gameTextsY 255


typedef enum { DIR_up, DIR_down, DIR_left, DIR_right, DIR_none } directions;

typedef enum { GAME_chooseNumBoards, GAME_AIorPlayer, GAME_game } gameStatuses;


typedef struct {
	int r, c, finalR;
	int availableMovement[4];
	directions choosenMovement;
	int remainingWalls;
	int color;
} player_t;

typedef player_t *player;


typedef struct {
	int centerR, centerC;
	bool horiz;
} wallPos;

typedef enum { todo, exploring, done } exploreCell;


/*
	GAME_reduceTimer
	---------------------------------------------------------------------
	Decreases the timer by 1 second
*/
void GAME_reduceTimer(void);


/*
	GAME_start
	---------------------------------------------------------------------
	Starts the game
*/
void GAME_start(void);


/*
	GAME_init
	---------------------------------------------------------------------
	Initializes everything needed for the game
*/
void GAME_init(void);


/*
	GAME_move
	---------------------------------------------------------------------
	Manages the player selecting a joystick move
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the direction the joystick was moved
*/
void GAME_move(directions dir);


/*
	GAME_switchPlayerWall
	---------------------------------------------------------------------
	Switches between moving the player and inserting a wall
*/
void GAME_switchPlayerWall(void);


/*
	GAME_rotateWall
	---------------------------------------------------------------------
	Rotates the temporary wall
*/
void GAME_rotateWall(void);


/*
	GAME_redrawCellsAfterText
	---------------------------------------------------------------------
	Re-draws the cells covered by the notification text
*/
void GAME_redrawCellsAfterText(void);


/*
	GAME_endOfTurn
	---------------------------------------------------------------------
	Signals that the user pressed the "end of turn" button
*/
void GAME_endOfTurn(void);


/*
	GAME_choiceMenu
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
void GAME_choiceMenu(char *row1, char *row2, char *opt1, char *opt2);


/*
	GAME_chooseMenu
	---------------------------------------------------------------------
	Allows the user to choose an option of a menu
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the option choosen (values other than DIR_up and DIR_down
			are ignored)
*/
void GAME_chooseMenu(directions dir);

#endif