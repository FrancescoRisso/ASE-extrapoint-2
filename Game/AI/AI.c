#include "AI.h"


bool moveIsTimerRunout = false;

extern int lastMove;
extern wallPos tmpWall;
extern bool isInsertingWall;
extern player players[];
extern int nowPlaying;
extern int playerPositionHistoryRow[], playerPositionHistoryCol[];


int AI_random(int max) {
	int i, sum = 0;

	for(i = 0; i < randomIterations; i++) sum += LPC_RIT->RICOUNTER % (max + 1);
	return sum % (max + 1);
}


void AI_move(int timeLeft) {
	// choose randomly if the move should be done now or later in the
	// turn (to emulate a player thinking)
	// if this is the last second before the timer expires, however,
	// the move is done with 100% probability
	if(!moveIsTimerRunout && (timeLeft == 1 || AI_random(1))) {
		AI_tryMirroringMove();

		if(!moveIsTimerRunout) {
			moveIsTimerRunout = false;
			GAME_endOfTurn();
		}
	};
}

bool AI_tryMirroringMove() {
	directions dir;
	int lastR = playerPositionHistoryRow[playerPositionHistoryCnt - 3];
	int lastC = playerPositionHistoryCol[playerPositionHistoryCnt - 3];
	int move = lastMove;
	decodeMove();

	move = playerID;  // to avoid "unused playerID" warning

	if(!isValid) return false;

	if(AI_random(randomMoveWithPreviousValid) == 0) return false;

	if(isWall) {
		if(players[nowPlaying]->remainingWalls == 0) return false;

		tmpWall.centerR = gridSize - posY - 2;
		tmpWall.centerC = gridSize - posX - 2;
		tmpWall.horiz = (bool) !vert;

		if(GAME_tmpWallOverlaps()) return false;
		if(!GAME_checkReachability(players[0], players[1])) return false;
		if(!GAME_checkReachability(players[1], players[0])) return false;

		isInsertingWall = true;
		return true;
	}

	dir = DIR_opposite(DIR_computeFromPoints(lastR, lastC, posY, posX));
	if(dir == DIR_none) return false;
	if(players[nowPlaying]->availableMovement[dir] == 0) return false;

	GAME_move(dir);
	return true;
}
