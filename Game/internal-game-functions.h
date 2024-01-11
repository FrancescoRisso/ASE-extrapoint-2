#ifndef __INTERNAL_GAME_H
#define __INTERNAL_GAME_H

#include "game.h"


/*
	GAME_drawTile
	---------------------------------------------------------------------
	Draws one of the tiles of the grid
	---------------------------------------------------------------------
	PARAMETERS:
		- r, c: the row and column of the tile
		- color: the fillColor
*/
void GAME_drawTile(int r, int c, int color);


/*
	GAME_drawGameTexts
	---------------------------------------------------------------------
	Writes all the text that does not need changes during the match
*/
void GAME_drawGameTexts(void);


/*
	GAME_drawNewTimer
	---------------------------------------------------------------------
	Resets the timer to 20 and draws the full bar
	---------------------------------------------------------------------
	PARAMETERS:
		- player: the player whose turn is starting
*/
void GAME_drawNewTimer(int player);


/*
	GAME_writeNumWalls
	---------------------------------------------------------------------
	Writes to the screen the number of walls of each player
*/
void GAME_writeNumWalls(void);


/*
	GAME_drawTileOffset
	---------------------------------------------------------------------
	Like GAME_drawTile, but the cell is computed as offset in one
	direction from another cell
	---------------------------------------------------------------------
	PARAMETERS:
		- baseR, baseC: the row and colum of the base cell
		- offset: the number of displacement cells
		- dir: the direction of the displacement
		- color: the color of the cell
*/
void GAME_drawTileOffset(int baseR, int baseC, int offset, directions dir, int color);


/*
	GAME_resetMovements
	---------------------------------------------------------------------
	Resets a player's availableMovements array and clears the respective
	cells
	---------------------------------------------------------------------
	PARAMETERS:
		- p: the player
*/
void GAME_resetMovements(player p);

/*
	GAME_findMovements
	---------------------------------------------------------------------
	Computes which movements a player can do, writes it to its
	availableMovement array and colors the corresponding tiles
	---------------------------------------------------------------------
	PARAMETERS:
		- p: the player
		- other: the opponent (that may need to be jumped)
*/
void GAME_findMovements(player p, player other);


/*
	GAME_findMovementDir
	---------------------------------------------------------------------
	Computes how much should a player move, if he moves in a given
	direction
	---------------------------------------------------------------------
	PARAMETERS:
		- p: the player
		- dir: the chosen direction
		- other: the opponent (that may need to be jumped)
*/
int GAME_findMovementDir(player p, directions dir, player other);


/*
	GAME_getOppositeDir
	---------------------------------------------------------------------
	Returns the opposite of a given direction (eg up <--> down)
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the given direction
	---------------------------------------------------------------------
	OUTPUT:
		- its opposite (with DIR_none <--> DIR_none)
*/
directions GAME_getOppositeDir(directions dir);


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
	GAME_end
	---------------------------------------------------------------------
	Ends the game, tells who the winner is, and to press int0 to restart
	---------------------------------------------------------------------
	PARAMETERS:
		- winner: the winning player
*/
void GAME_end(player winner);


/*
	GAME_drawGameTexts
	---------------------------------------------------------------------
	Draws the "Now playing" and "Walls left" labels
*/
void GAME_drawGameTexts(void);


/*
	GAME_initPlayers
	---------------------------------------------------------------------
	Initializes all the player data
*/
void GAME_initPlayers(void);


/*
	GAME_drawEmptyGrid
	---------------------------------------------------------------------
	Draws the empty grid
*/
void GAME_drawEmptyGrid(void);


/*
	GAME_encodePlayerMove
	---------------------------------------------------------------------
	Encodes with the custom format the fact that a player moved
	Then, if the game is being played with two boards and the move was
	done by "this board"'s player, the encoded value is sent to the other
	board
	---------------------------------------------------------------------
	PARAMETERS:
		- playerID: the index of the player that just played
*/
void GAME_encodePlayerMove(int playerID);


/*
	GAME_encodeWallPlacement
	---------------------------------------------------------------------
	Encodes with the custom format the fact that a wall was inserted
	Then, if the game is being played with two boards and the move was
	done by "this board"'s player, the encoded value is sent to the other
	board
	---------------------------------------------------------------------
	PARAMETERS:
		- playerID: the index of the player that just played
*/
void GAME_encodeWallPlacement(int playerID);


/*
	GAME_encodeSkipTurn
	---------------------------------------------------------------------
	Encodes with the custom format the fact that a player run out of time
	Then, if the game is being played with two boards and the move was
	done by "this board"'s player, the encoded value is sent to the other
	board
	---------------------------------------------------------------------
	PARAMETERS:
		- playerID: the index of the player that just played
*/
void GAME_encodeSkipTurn(int playerID);


/*
	GAME_movePlayer
	---------------------------------------------------------------------
	Moves the active player in a chosen direction, without confirming it
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the direction in which to move
*/
void GAME_movePlayer(directions dir);


/*
	GAME_drawWall
	---------------------------------------------------------------------
	Draws a wall
	---------------------------------------------------------------------
	PARAMETERS:
		- wall: the specifics of the wall to be drawn
		- color: the color of the desired wall
*/
void GAME_drawWall(wallPos wall, int color);

/*
	GAME_deleteTmpWall
	---------------------------------------------------------------------
	Deletes the temporary wall from the screen, re-drawing walls under it
*/
void GAME_deleteTmpWall(void);


/*
	GAME_moveWall
	---------------------------------------------------------------------
	Moves the temporary wall in a given direction, if possible
	---------------------------------------------------------------------
	PARAMETERS:
		- dir: the direction where to move
*/
void GAME_moveWall(directions dir);


/*
	GAME_movementBlockedByWall
	---------------------------------------------------------------------
	Tells if there are some walls that block a movement
	---------------------------------------------------------------------
	PARAMETERS:
		- startR: the starting row
		- startC: the starting column
		- vertical: if the movement is along the vertical direction
		- increasing: if the movement is towards positive coordinates
	---------------------------------------------------------------------
	OUTPUT:
		- if a wall blocks that movement
*/
bool GAME_movementBlockedByWall(int startR, int startC, bool vertical, bool increasing);


/*
	GAME_notifyText
	---------------------------------------------------------------------
	Writes a 2-row text in the middle of the board, and starts a timer to
	remove it after 1 second
	---------------------------------------------------------------------
	PARAMETERS:
		- row1: the content of the first row
		- row2: the content of the second row
*/
void GAME_notifyText(char* row1, char* row2);


/*
	GAME_wallOverlap
	---------------------------------------------------------------------
	Returns whether two walls overlap with each other
	---------------------------------------------------------------------
	PARAMETERS:
		- strict: if set, the overlap is considered "pixel by pixel".
			If not set, a "segment by segment" overlap is considered
			instead
	---------------------------------------------------------------------
	OUTPUT:
		- if there is an overlap or not
*/
bool GAME_wallOverlap(wallPos wall1, wallPos wall2, bool strict);


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


/*
	GAME_changeTurn
	---------------------------------------------------------------------
	Passes the turn to the opposite player
*/
void GAME_changeTurn(void);

#endif
