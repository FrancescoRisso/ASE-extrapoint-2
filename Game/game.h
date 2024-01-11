#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <string.h>

#include "../CAN/CAN.h"
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"
#include "../directions/directions.h"
#include "../timers/timers.h"
#include "../utils/abs.h"
#include "../utils/bool.h"
#include "AI/AI.h"
#include "lpc17xx.h"
#include "menu.h"



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
#define errorTextColor Red
#define selectedMenuColor Yellow


#define timerX 130
#define timerY 285
#define timerH 15
#define timerWperVal 5


#define gameTextsY 255


#define playerPositionHistoryCnt 3


#define decodeMove()                            \
	int playerID;                               \
	int posX, posY;                             \
	bool isValid, isWall, vert;                 \
	playerID = move & 0xFF000000;               \
	isWall = (bool) ((move & 0x00F00000) != 0); \
	vert = (bool) ((move & 0x000F0000) == 0);   \
	isValid = (bool) !((!isWall) && (!vert));    \
	posY = (move & 0x0000FF00) >> 8;            \
	posX = move & 0x000000FF


typedef enum { GAME_chooseNumBoards, GAME_AIorPlayer, GAME_game } gameStatuses;


typedef enum { PLAYER_player, PLAYER_ai, PLAYER_otherBoard } playerTypes;

typedef struct {
	int r, c, finalR;
	int availableMovement[8];
	directions choosenMovement;
	int remainingWalls;
	int color;
	playerTypes playerType;
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
	GAME_oneBoardGame
	---------------------------------------------------------------------
	Sets the game to use a sigle board
*/
void GAME_oneBoardGame(void);


/*
	GAME_twoBoardGame
	---------------------------------------------------------------------
	Sets the game to use two boards
	---------------------------------------------------------------------
	PARAMETERS:
		- send: if the other board shoudl be notified
*/
void GAME_twoBoardGame(bool send);


/*
	GAME_setPlayerType
	---------------------------------------------------------------------
	Sets if the player/enemy (dual/mono board) is human or NPC
	---------------------------------------------------------------------
	PARAMETERS:
		- isHuman: if the selected player is a human
*/
void GAME_setPlayerType(bool isHuman);


/*
	GAME_execEncodedMove
	---------------------------------------------------------------------
	Executes a nove encoded with the shared format, and ends the turn
	---------------------------------------------------------------------
	PARAMETERS:
		- move: the encoded move
*/
void GAME_execEncodedMove(int move);


/*
	GAME_notifyMissingBoard
	---------------------------------------------------------------------
	Prints a message telling that the match is in single-board mode due
	to a failed handshake
*/
void GAME_notifyMissingBoard(void);


/*
	GAME_stopTimersAndRIT
	---------------------------------------------------------------------
	Temporarily stops timers and RIT
*/
void GAME_stopTimersAndRIT(void);


/*
	GAME_continueTimersAndRIT
	---------------------------------------------------------------------
	Resumes the timers and RIT's execution
*/
void GAME_continueTimersAndRIT(void);


/*
	GAME_tmpWallOverlaps
	---------------------------------------------------------------------
	Tells if the currently-placed wall overlaps (segmentwise) with some
	other walls
	---------------------------------------------------------------------
	OUTPUT:
		- if there are overlaps
*/
bool GAME_tmpWallOverlaps(void);


/*
	GAME_checkReachability
	---------------------------------------------------------------------
	Checks if it is possible for a given player to reach their end
	---------------------------------------------------------------------
	PARAMETER:
		- p: the player where to start
		- other: the position of the opponent
	---------------------------------------------------------------------
	OUTPUT:
		- if a path exists or not
*/
bool GAME_checkReachability(player p, player other);


#endif
