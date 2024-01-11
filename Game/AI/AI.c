#include "AI.h"


bool moveIsTimerRunout = false;

extern int lastMove;
extern wallPos tmpWall;
extern bool isInsertingWall;
extern player players[];
extern int nowPlaying;
extern int playerPositionHistoryRow[], playerPositionHistoryCol[];


int AI_random(int max) {
	return rand() % (max + 1);
}


void AI_move(int timeLeft) {
	// choose randomly if the move should be done now or later in the
	// turn (to emulate a player thinking)
	// if this is the last second before the timer expires, however,
	// the move is done with 100% probability
	if(!moveIsTimerRunout && (timeLeft == 1 || AI_random(2) == 0)) {
		if(!AI_tryMirroringMove()) AI_randomAction();

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

		if(!AI_wallIsCorrect()) return false;

		isInsertingWall = true;
		return true;
	}

	dir = DIR_opposite(DIR_computeFromPoints(lastR, lastC, posY, posX));
	if(dir == DIR_none) return false;
	if(players[nowPlaying]->availableMovement[dir] == 0) return false;

	GAME_move(dir);
	return true;
}


void AI_randomAction() {
	int totWeight, randVal;

	totWeight = randomWeightOfDoingNothing + randomWeightOfMovingToken - 1;
	if(players[nowPlaying]->remainingWalls != 0) totWeight += randomWeightOfPlacingWall;

	randVal = AI_random(totWeight);

	if(randVal < randomWeightOfDoingNothing) {
		moveIsTimerRunout = true;
		return;
	}

	randVal -= randomWeightOfDoingNothing;

	if(randVal < randomWeightOfMovingToken) {
		AI_moveRandomly();
		return;
	}

	randVal -= randomWeightOfMovingToken;

	// placeWall
}

void AI_moveRandomly() {
	int totWeight = 0;
	int weights[DIR_none];
	int weightUp, weightDown;
	int randVal;
	directions dir;

	if(players[nowPlaying]->finalR == 0) {
		weightDown = randomWeightOfWrongDir;
		weightUp = randomWeightOfCorrectDir;
	} else {
		weightDown = randomWeightOfCorrectDir;
		weightUp = randomWeightOfWrongDir;
	}

	weights[DIR_up] = weightUp;
	weights[DIR_up_left] = weightUp;
	weights[DIR_up_right] = weightUp;
	weights[DIR_down] = weightDown;
	weights[DIR_down_left] = weightDown;
	weights[DIR_down_right] = weightDown;
	weights[DIR_left] = randomWeightOfUselessDir;
	weights[DIR_right] = randomWeightOfUselessDir;

	for(dir = (directions) 0; dir < DIR_none; dir++)
		if(players[nowPlaying]->availableMovement[dir] != 0) totWeight += weights[dir];

	randVal = AI_random(totWeight - 1);

	for(dir = (directions) 0; dir < DIR_none; dir++)
		if(players[nowPlaying]->availableMovement[dir] != 0) {
			if(randVal < weights[dir]) {
				GAME_move(dir);
				return;
			} else
				randVal -= weights[dir];
		}
}


bool AI_wallIsCorrect() {
	if(GAME_tmpWallOverlaps()) return false;
	if(!GAME_checkReachability(players[0], players[1])) return false;
	if(!GAME_checkReachability(players[1], players[0])) return false;
	
	return true;
}

		}
}
