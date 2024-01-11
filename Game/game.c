#include "game.h"

#include "internal-game-functions.h"


bool isFirstGame = true;

player_t playersData[2];
player players[2] = {playersData, playersData + 1};
int nowPlaying;

bool isInsertingWall = false;

wallPos tmpWall;
bool tmpWallIsValid = false;
wallPos insertedWalls[16];
int numInsertedWalls = 0;

int timerCnt = 20;

extern bool joystickEnabled;
extern bool int0Enabled;
extern bool key1Enabled;
extern bool key2Enabled;

gameStatuses gameStatus = GAME_chooseNumBoards;

bool dualBoard;
bool otherPlayerReady = false;
bool ready;
bool handshakeDone = false;

int myID = -1;

void GAME_drawTile(int r, int c, int color) {
	int left, right, top, bottom, wall;

	left = (c + 1) * spaceSize + c * squareSize;
	right = (c + 1) * (spaceSize + squareSize);
	top = (r + 1) * spaceSize + r * squareSize;
	bottom = (r + 1) * (spaceSize + squareSize);

	LCD_drawRect(left, right, top, bottom, gridColor, color);

	for(wall = 0; wall < numInsertedWalls; wall++) {
		if(insertedWalls[wall].centerR == r || insertedWalls[wall].centerR == r - 1)
			if(insertedWalls[wall].centerC == c || insertedWalls[wall].centerC == c - 1) GAME_drawWall(insertedWalls[wall], fixedWallColor);
	}

	if(tmpWallIsValid)
		if(tmpWall.centerR == r || tmpWall.centerR == r - 1)
			if(tmpWall.centerC == c || tmpWall.centerC == c - 1) GAME_drawWall(tmpWall, placingWallColor);
}


void GAME_initPlayers() {
	player p;

	players[1]->r = players[0]->finalR = 0;
	players[0]->r = players[1]->finalR = gridSize - 1;

	players[0]->c = players[1]->c = gridSize / 2;

	players[0]->choosenMovement = players[1]->choosenMovement = DIR_none;

	players[0]->remainingWalls = players[1]->remainingWalls = numWalls;

	players[1]->color = player1Color;
	players[0]->color = player2Color;

	nowPlaying = 0;
	p = players[0];
	GAME_resetMovements(p);
	GAME_drawTile(p->r, p->c, p->color);
	p = players[1];
	GAME_resetMovements(p);
	GAME_drawTile(p->r, p->c, p->color);
}


void GAME_drawEmptyGrid(void) {
	int r, c;

	LCD_Clear(White);

	for(r = 0; r < gridSize; r++)
		for(c = 0; c < gridSize; c++) GAME_drawTile(r, c, backgroundColor);
}


void GAME_reduceTimer(void) {
	int left, right;

	right = timerX + timerCnt * timerWperVal;
	left = timerX + (--timerCnt) * timerWperVal;

	LCD_drawRect(left, right + 1, timerY - 1, timerY + timerH + 1, backgroundColor, backgroundColor);

	if(timerCnt == 0) {
		GAME_movePlayer(GAME_getOppositeDir(players[nowPlaying]->choosenMovement));
		GAME_changeTurn();
	}
}


void GAME_drawGameTexts(void) {
	GUI_Text(135, gameTextsY, (uint8_t *) "Now playing:", textColor, backgroundColor);
	GUI_Text(18, gameTextsY, (uint8_t *) "Walls left:", textColor, backgroundColor);
	GAME_writeNumWalls();
}


void GAME_drawNewTimer(int player) {
	int color = players[player]->color;
	timerCnt = 20;

	LCD_drawRect(timerX, timerX + 20 * timerWperVal, timerY, timerY + timerH, color, color);
}


void GAME_writeNumWalls(void) {
	char buf[2];

	// LCD_drawRect(38, 78, timerY - 10, timerY + 10, backgroundColor, Red);

	sprintf(buf, "%d", players[0]->remainingWalls);
	GUI_Text(48, timerY, (uint8_t *) buf, players[0]->color, backgroundColor);

	sprintf(buf, "%d", players[1]->remainingWalls);
	GUI_Text(68, timerY, (uint8_t *) buf, players[1]->color, backgroundColor);
}


void GAME_drawTileOffset(int baseR, int baseC, int offset, directions dir, int color) {
	switch(dir) {
		case DIR_down: GAME_drawTile(baseR + offset, baseC, color); break;
		case DIR_up: GAME_drawTile(baseR - offset, baseC, color); break;
		case DIR_left: GAME_drawTile(baseR, baseC - offset, color); break;
		case DIR_right: GAME_drawTile(baseR, baseC + offset, color); break;
		default: break;
	}
}


void GAME_endOfTurn(void) {
	player lastP = players[nowPlaying];

	GAME_stopTimersAndRIT();

	if(isInsertingWall) {
		if(GAME_tmpWallOverlaps()) {
			GAME_notifyText("Invalid position", "(overlap)");
			return;
		}
		if(!GAME_checkReachability(players[0], players[1]) || !GAME_checkReachability(players[1], players[0])) {
			GAME_notifyText("Invalid position", "(path is blocked)");
			return;
		}
		GAME_drawWall(tmpWall, fixedWallColor);
		insertedWalls[numInsertedWalls++] = tmpWall;
		lastP->remainingWalls--;
		GAME_writeNumWalls();

		GAME_movePlayer(GAME_getOppositeDir(lastP->choosenMovement));

		GAME_encodeWallPlacement(nowPlaying);
	} else
		GAME_encodePlayerMove(nowPlaying);

	GAME_changeTurn();
}


void GAME_changeTurn(void) {
	player lastP = players[nowPlaying];

	GAME_stopTimersAndRIT();

	if(timerCnt == 0) {
		GAME_movePlayer(GAME_getOppositeDir(lastP->choosenMovement));
		GAME_deleteTmpWall();
		GAME_encodeSkipTurn(nowPlaying);
	}

	isInsertingWall = false;
	tmpWallIsValid = false;

	nowPlaying = (nowPlaying + 1) % 2;
	GAME_resetMovements(lastP);

	if(lastP->r == lastP->finalR) {
		GAME_end(lastP);
		isFirstGame = false;
		return;
	}
	GAME_drawNewTimer(nowPlaying);

	if(players[nowPlaying]->playerType != PLAYER_otherBoard) GAME_findMovements(players[nowPlaying], lastP);

	GAME_continueTimersAndRIT();
}


void GAME_start(void) {
	int0Enabled = false;
	joystickEnabled = true;
	key1Enabled = true;
	nowPlaying = 1;
	GAME_changeTurn();
	TIMER_enable(TIMER_0);
}


void GAME_resetMovements(player p) {
	int i;

	for(i = 0; i < 4; i++)
		if(p->availableMovement[i] != 0 && p->choosenMovement != i)
			GAME_drawTileOffset(p->r, p->c, p->availableMovement[i], (directions) i, backgroundColor);

	switch(p->choosenMovement) {
		case DIR_down: p->r += p->availableMovement[DIR_down]; break;
		case DIR_up: p->r -= p->availableMovement[DIR_up]; break;
		case DIR_left: p->c -= p->availableMovement[DIR_left]; break;
		case DIR_right: p->c += p->availableMovement[DIR_right]; break;
		default: break;
	}

	// GAME_drawTile(p->r, p->c, p->color);

	p->choosenMovement = DIR_none;
	for(i = 0; i < 4; i++) p->availableMovement[i] = 0;
}


void GAME_findMovements(player p, player other) {
	directions dir = DIR_down;
	int res;

	for(dir = (directions) 0; dir < 4; dir++) {
		res = GAME_findMovementDir(p, dir, other);
		p->availableMovement[dir] = res;
		if(res && p->playerType == PLAYER_player) GAME_drawTileOffset(p->r, p->c, res, dir, availableMoveColor);
	}
}


int GAME_findMovementDir(player p, directions dir, player other) {
	int finalR, finalC, afterJump;

	finalR = p->r;
	finalC = p->c;

	// try moving by 1
	switch(dir) {
		case DIR_down: finalR += 1; break;
		case DIR_up: finalR -= 1; break;
		case DIR_left: finalC -= 1; break;
		case DIR_right: finalC += 1; break;
		default: break;
	}

	// movement by 1 is outside grid
	if(finalR < 0 || finalR >= gridSize) return 0;
	if(finalC < 0 || finalC >= gridSize) return 0;

	// movement by 1 is blocked by wall
	if(GAME_movementBlockedByWall(p->r, p->c, (bool) (dir == DIR_up || dir == DIR_down), (bool) (dir == DIR_down || dir == DIR_right))) return 0;

	// movement by 1 is blocked by player
	if(finalR == other->r && finalC == other->c) {
		afterJump = GAME_findMovementDir(other, dir, p);

		// other player can be jumped
		if(afterJump != 0) return 2;

		// other player cannot be jumped
		else
			return 0;
	}

	// if none of the previous conditions sussist, movement by 1 is allowed
	return 1;
}


void GAME_movePlayer(directions dir) {
	player p = players[nowPlaying];

	if(dir == DIR_none) return;

	GAME_stopTimersAndRIT();

	// if player is reverting its last tmpMove
	if(dir == GAME_getOppositeDir(p->choosenMovement)) {
		GAME_drawTileOffset(p->r, p->c, p->availableMovement[p->choosenMovement], p->choosenMovement, availableMoveColor);
		GAME_drawTile(p->r, p->c, p->color);
		p->choosenMovement = DIR_none;
	} else if(dir != p->choosenMovement) {
		//
		if(p->availableMovement[dir] == 0) {
			GAME_continueTimersAndRIT();
			return;
		}
		if(p->choosenMovement == DIR_none)
			GAME_drawTile(p->r, p->c, backgroundColor);
		else
			GAME_drawTileOffset(p->r, p->c, p->availableMovement[p->choosenMovement], p->choosenMovement, availableMoveColor);
		GAME_drawTileOffset(p->r, p->c, p->availableMovement[dir], dir, p->color);
		p->choosenMovement = dir;
	}

	GAME_continueTimersAndRIT();
}


directions GAME_getOppositeDir(directions dir) {
	switch(dir) {
		case DIR_down: return DIR_up;
		case DIR_up: return DIR_down;
		case DIR_left: return DIR_right;
		case DIR_right: return DIR_left;
		default: return DIR_none;
	}
}


void GAME_stopTimersAndRIT(void) {
	TIMER_disable(TIMER_0);
	disable_RIT();
}


void GAME_continueTimersAndRIT(void) {
	if(players[nowPlaying]->playerType != PLAYER_otherBoard) TIMER_enable(TIMER_0);
	enable_RIT();
}


void GAME_end(player winner) {
	int xPos = 50, yPos = 260, winnerXoffset = 120, winnerSize = 15;

	joystickEnabled = key1Enabled = key2Enabled = false;

	GAME_stopTimersAndRIT();

	LCD_drawRect(0, 240, 240, 320, backgroundColor, backgroundColor);

	GUI_Text(xPos, yPos, "The winner is: ", textColor, backgroundColor);
	LCD_drawRect(xPos + winnerXoffset, xPos + winnerXoffset + winnerSize, yPos, yPos + winnerSize, backgroundColor, winner->color);

	GUI_Text(40, yPos + 30, "Press RESET to restart", textColor, backgroundColor);

	enable_RIT();
}


void GAME_init(void) {
	LCD_Clear(backgroundColor);

	numInsertedWalls = 0;

	GAME_drawEmptyGrid();
	GAME_initPlayers();
	GAME_drawGameTexts();

	NVIC_SetPriority(RIT_IRQn, 2);
	NVIC_SetPriority(TIMER0_IRQn, 1);

	TIMER_setValue(TIMER_0, TIMER_matchReg0, 25000000, TIMER_reset_interrupt);
	// TIMER_setValue(TIMER_0, TIMER_matchReg0, 100000000, TIMER_reset_interrupt);
	enable_RIT();

	gameStatus = GAME_game;

	GAME_start();
}


void GAME_encodePlayerMove(int playerID) {
	int res = 0;
	player p = players[playerID];

	if(timerCnt == 0) {
		GAME_encodeSkipTurn(playerID);
		return;
	}

	res = playerID;           // playerID
	res = (res << 4) + 0;     // playerMove
	res = (res << 4) + 0;     // wallDirection defaults to 0
	res = (res << 8) + p->r;  // player Y
	res = (res << 8) + p->c;  // player X

	if(dualBoard && playerID == myID) CAN_wrMsg(res);
}


void GAME_encodeWallPlacement(int playerID) {
	int res = 0;

	if(timerCnt == 0) {
		GAME_encodeSkipTurn(playerID);
		return;
	}

	res = playerID;                      // playerID
	res = (res << 4) + 1;                // wallPlacement
	res = (res << 4) + tmpWall.horiz;    // wallDirection
	res = (res << 8) + tmpWall.centerR;  // wall Y
	res = (res << 8) + tmpWall.centerC;  // wall X

	if(dualBoard && playerID == myID) CAN_wrMsg(res);
}


void GAME_encodeSkipTurn(int playerID) {
	int res = 0;

	res = playerID;        // playerID
	res = (res << 4) + 0;  // playerMove = 0
	res = (res << 4) + 1;  // wallDirection = 1
	res = (res << 8) + 0;  // Y = 0
	res = (res << 8) + 0;  // X = 0

	if(dualBoard && playerID == myID) CAN_wrMsg(res);
}


void GAME_move(directions dir) {
	if(isInsertingWall)
		GAME_moveWall(dir);
	else
		GAME_movePlayer(dir);
}


void GAME_switchPlayerWall(void) {
	if(isInsertingWall) {
		GAME_deleteTmpWall();
		isInsertingWall = false;
		key2Enabled = false;
	} else {
		if(players[nowPlaying]->remainingWalls == 0) {
			GAME_notifyText("No walls available,", "move the token");
			return;
		}
		if(!tmpWallIsValid) {
			tmpWall.centerC = tmpWall.centerR = gridSize / 2;
			tmpWall.horiz = tmpWallIsValid = true;
		}
		GAME_drawWall(tmpWall, placingWallColor);
		isInsertingWall = true;
		key2Enabled = true;
	}
}


void GAME_drawWall(wallPos wall, int color) {
	/*
		Assume a grid like this, where x is the wallcenter:
		 __   __   __
		|__| |__| |__|
		 __   __ x __
		|__| |__| |__|

		Variables left & top identify the bottom-right corner of the
		top-left square related to x

		Walls will therefore be:
		- horizontal: (left - squareSize, top) x (left + squareSize + spaceSize, top)
		- vertical: (left, top - squareSize) x (left, top + squareSize + spaceSize)
	*/

	int left = (wall.centerC + 1) * (spaceSize + squareSize);
	int top = (wall.centerR + 1) * (spaceSize + squareSize);

	if(wall.horiz)
		LCD_drawRect(left - squareSize, left + squareSize + spaceSize, top, top + spaceSize, color, color);
	else
		LCD_drawRect(left, left + spaceSize, top - squareSize, top + spaceSize + squareSize, color, color);
}


void GAME_deleteTmpWall(void) {
	// for left and top, refer to GAME_drawWall
	int left = (tmpWall.centerC + 1) * (spaceSize + squareSize);
	int top = (tmpWall.centerR + 1) * (spaceSize + squareSize);
	int i;

	if(!isInsertingWall) return;

	if(tmpWall.horiz) {
		LCD_drawRect(left - squareSize, left + squareSize + spaceSize, top, top + spaceSize, backgroundColor, backgroundColor);

		LCD_DrawLine(left - squareSize, top, left, top, gridColor);
		LCD_DrawLine(left + squareSize + spaceSize, top, left + spaceSize, top, gridColor);
		LCD_DrawLine(left - squareSize, top + spaceSize, left, top + spaceSize, gridColor);
		LCD_DrawLine(left + squareSize + spaceSize, top + spaceSize, left + spaceSize, top + spaceSize, gridColor);
	} else {
		LCD_drawRect(left, left + spaceSize, top - squareSize, top + spaceSize + squareSize, backgroundColor, backgroundColor);

		LCD_DrawLine(left, top - squareSize, left, top, gridColor);
		LCD_DrawLine(left, top + squareSize + spaceSize, left, top + spaceSize, gridColor);
		LCD_DrawLine(left + spaceSize, top - squareSize, left + spaceSize, top, gridColor);
		LCD_DrawLine(left + spaceSize, top + squareSize + spaceSize, left + spaceSize, top + spaceSize, gridColor);
	}

	for(i = 0; i < numInsertedWalls; i++)
		if(GAME_wallOverlap(tmpWall, insertedWalls[i], true)) GAME_drawWall(insertedWalls[i], fixedWallColor);
}


void GAME_rotateWall(void) {
	GAME_deleteTmpWall();
	tmpWall.horiz = (bool) ((tmpWall.horiz + 1) % 2);
	GAME_drawWall(tmpWall, placingWallColor);
}


void GAME_moveWall(directions dir) {
	GAME_deleteTmpWall();

	switch(dir) {
		case DIR_up:
			if(tmpWall.centerR != 0) tmpWall.centerR--;
			break;
		case DIR_down:
			if(tmpWall.centerR != gridSize - 2) tmpWall.centerR++;
			break;
		case DIR_left:
			if(tmpWall.centerC != 0) tmpWall.centerC--;
			break;
		case DIR_right:
			if(tmpWall.centerC != gridSize - 2) tmpWall.centerC++;
			break;
	}

	GAME_drawWall(tmpWall, placingWallColor);
}


bool GAME_movementBlockedByWall(int startR, int startC, bool vertical, bool increasing) {
	int wall;

	// if increasing, consider the opposite movement
	// used to reduce cases to having walls only above or to the left
	if(increasing) {
		if(vertical)
			startR++;
		else
			startC++;
	}

	for(wall = 0; wall < numInsertedWalls; wall++) {
		if(vertical && insertedWalls[wall].horiz && insertedWalls[wall].centerR == (startR - 1) &&
			(insertedWalls[wall].centerC == startC || insertedWalls[wall].centerC == startC - 1))
			return true;
		if(!vertical && !insertedWalls[wall].horiz && insertedWalls[wall].centerC == (startC - 1) &&
			(insertedWalls[wall].centerR == startR || insertedWalls[wall].centerR == startR - 1))
			return true;
	}

	return false;
}


void GAME_notifyText(char *row1, char *row2) {
	POW_setPeripheralOn(POW_timer1);
	TIMER_setValue(TIMER_1, TIMER_matchReg0, 25000000, TIMER_stop_reset_interrupt);
	// TIMER_enableReset(TIMER_2, true);
	// TIMER_enableReset(TIMER_2, false);

	GUI_Text((240 - 8 * strlen(row1)) / 2, 110, (uint8_t *) row1, textColor, backgroundColor);
	GUI_Text((240 - 8 * strlen(row2)) / 2, 130, (uint8_t *) row2, textColor, backgroundColor);

	TIMER_enable(TIMER_1);
}


void GAME_redrawCellsAfterText(void) {
	int colors[2][gridSize];
	int r, c;
	player p = players[nowPlaying];

	// default grid cell is empty
	for(r = 3; r < 5; r++)
		for(c = 0; c < gridSize; c++) colors[r - 3][c] = backgroundColor;

	// mark current player and its movements
	if(p->r == 3 || p->r == 4) {
		colors[p->r - 3][p->c] = p->color;
		if(p->availableMovement[DIR_right] != 0) colors[p->r - 3][p->c + p->availableMovement[DIR_right]] = availableMoveColor;
		if(p->availableMovement[DIR_left] != 0) colors[p->r - 3][p->c - p->availableMovement[DIR_left]] = availableMoveColor;
	}
	if(p->r + p->availableMovement[DIR_down] == 3 || p->r + p->availableMovement[DIR_down] == 4)
		colors[p->r + p->availableMovement[DIR_down] - 3][p->c] = availableMoveColor;
	if(p->r - p->availableMovement[DIR_up] == 3 || p->r - p->availableMovement[DIR_up] == 4)
		colors[p->r - p->availableMovement[DIR_down] - 3][p->c] = availableMoveColor;

	// mark other player
	p = players[(nowPlaying + 1) % 2];
	if(p->r == 3 || p->r == 4) colors[p->r - 3][p->c] = p->color;

	// clear the background
	LCD_drawRect(0, 240, 110, 150, backgroundColor, backgroundColor);

	// apply colors to the grid
	for(r = 3; r < 5; r++)
		for(c = 0; c < gridSize; c++) GAME_drawTile(r, c, colors[r - 3][c]);

	GAME_continueTimersAndRIT();
}


bool GAME_wallOverlap(wallPos wall1, wallPos wall2, bool strict) {
	bool bothHoriz, bothVert, diffOrientat, sameCenterR, sameCenterC, similarCenterR, similarCenterC;
	bothHoriz = bothVert = diffOrientat = sameCenterR = sameCenterC = similarCenterR = similarCenterC = false;

	if(wall1.horiz && wall2.horiz) bothHoriz = true;
	if(!wall1.horiz && !wall2.horiz) bothVert = true;
	if(wall1.horiz != wall2.horiz) diffOrientat = true;

	if(wall1.centerC == wall2.centerC) sameCenterC = true;
	if(abs(wall1.centerC - wall2.centerC) <= 1) similarCenterC = true;
	if(wall1.centerR == wall2.centerR) sameCenterR = true;
	if(abs(wall1.centerR - wall2.centerR) <= 1) similarCenterR = true;

	if(bothHoriz && sameCenterR && similarCenterC) return true;
	if(bothVert && sameCenterC && similarCenterR) return true;
	if(diffOrientat && sameCenterR && sameCenterC) return true;
	if(diffOrientat && strict && similarCenterR && similarCenterC) return true;

	return false;
}


bool GAME_tmpWallOverlaps(void) {
	int i;
	for(i = 0; i < numInsertedWalls; i++)
		if(GAME_wallOverlap(tmpWall, insertedWalls[i], false)) return true;
	return false;
}


bool GAME_checkReachability(player p, player other) {
	exploreCell explored[gridSize][gridSize] = {todo};
	bool res = false, somethingChanged = true;
	int r, c;
	player_t tmpPlayer;

	explored[p->r][p->c] = exploring;

	insertedWalls[numInsertedWalls++] = tmpWall;

	while(somethingChanged && !res) {
		somethingChanged = false;

		for(r = 0; r < gridSize && !res; r++) {
			for(c = 0; c < gridSize && !res; c++) {
				if(explored[r][c] == exploring) {
					if(r == p->finalR) res = true;

					tmpPlayer.r = r;
					tmpPlayer.c = c;

					explored[r][c] = done;
					somethingChanged = true;

					if(GAME_findMovementDir(&tmpPlayer, DIR_up, &tmpPlayer) && explored[r - 1][c] == todo) explored[r - 1][c] = exploring;
					if(GAME_findMovementDir(&tmpPlayer, DIR_down, &tmpPlayer) && explored[r + 1][c] == todo) explored[r + 1][c] = exploring;
					if(GAME_findMovementDir(&tmpPlayer, DIR_left, &tmpPlayer) && explored[r][c - 1] == todo) explored[r][c - 1] = exploring;
					if(GAME_findMovementDir(&tmpPlayer, DIR_right, &tmpPlayer) && explored[r][c + 1] == todo) explored[r][c + 1] = exploring;
				}
			}
		}
	}

	--numInsertedWalls;
	return res;
}


void GAME_oneBoardGame() {
	dualBoard = false;
	MENU_playerTypeMenu();
	otherPlayerReady = true;
}


void GAME_twoBoardGame(bool send) {
	dualBoard = true;
	myID = 0;
	if(send) {
		CAN_wrMsg(1);
		TIMER_setValue(TIMER_1, TIMER_matchReg0, 25000000, TIMER_stop_reset_interrupt);
		TIMER_enable(TIMER_1);
	}
}


void GAME_setPlayerType(bool isHuman) {
	if(dualBoard) {
		players[myID]->playerType = isHuman ? PLAYER_player : PLAYER_ai;
		players[(myID + 1) % 2]->playerType = PLAYER_otherBoard;
		CAN_wrMsg((myID << 24));
	} else {
		players[0]->playerType = PLAYER_player;
		players[1]->playerType = isHuman ? PLAYER_player : PLAYER_ai;
	}

	ready = true;
	if(otherPlayerReady) GAME_init();
}


void GAME_execEncodedMove(int move) {
	int playerID;
	int posX, posY;
	bool isValid, isWall, vert;

	player p;

	playerID = move & 0xFF000000;
	isWall = (bool) ((move & 0x00F00000) != 0);
	vert = (bool) ((move & 0x000F0000) == 0);
	isValid = (bool) (!isWall && !vert);
	posY = (move & 0x0000FF00) >> 8;
	posX = move & 0x000000FF;

	p = players[playerID];

	if(isValid) {
		if(isWall) {
			tmpWall.centerR = posY;
			tmpWall.centerC = posX;
			tmpWall.horiz = (bool) !vert;
			isInsertingWall = true;
		} else {
			GAME_drawTile(p->r, p->c, backgroundColor);
			p->r = posY;
			p->c = posX;
			GAME_drawTile(p->r, p->c, p->color);
		}
	}

	GAME_endOfTurn();
}


void GAME_notifyMissingBoard() {
	char row1[] = "Playing with a single\0";
	char row2[] = "board, since no other\0";
	char row3[] = "board handshaked\0";

	GUI_Text((240 - 8 * strlen(row1)) / 2, 260, (uint8_t *) row1, errorTextColor, backgroundColor);
	GUI_Text((240 - 8 * strlen(row2)) / 2, 280, (uint8_t *) row2, errorTextColor, backgroundColor);
	GUI_Text((240 - 8 * strlen(row3)) / 2, 300, (uint8_t *) row3, errorTextColor, backgroundColor);
}
