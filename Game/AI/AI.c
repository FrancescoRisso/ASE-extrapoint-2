#include "AI.h"


extern int lastMove;
extern wallPos tmpWall;
extern bool isInsertingWall;
extern player players[];
extern int nowPlaying;
extern int playerPositionHistoryRow[], playerPositionHistoryCol[];
extern wallPos insertedWalls[];
extern int numInsertedWalls;

#ifdef useSecondAlgorithm
/*
	AI_findDistFromArrival
	---------------------------------------------------------------------
	Finds the minimum number of moves for a player to reach its
	destination.
	This is done by writing in a matrix the minimum distance to reach
	each cell (until the final row is reached)
	---------------------------------------------------------------------
	PARAMETERS:
		- dist: a matrix where the function can write the distances.
			Passed as parameter in order for the calling function to be
			able to read all the distances
		- p: the player where to start
		- p: the opponent
	---------------------------------------------------------------------
	OUTPUT:
		- the minimum number of moves that p needs to reach its target
*/
int AI_findDistFromArrival(int dist[gridSize][gridSize], player p, player other);
#endif


int AI_random(int max) {
	return rand() % (max + 1);
}


void AI_move(int timeLeft) {
	// don't do a move immediately
	if(timeLeft == 19) return;

	// choose randomly if the move should be done now or later in the
	// turn (to emulate a player thinking)
	if(AI_random(randomWeightOfPlayingLater) == 0) {
#ifdef useFirstAlgorithm
		if(!AI_tryMirroringMove()) AI_randomAction();
#endif

#ifdef useSecondAlgorithm
		AI_randomAction();
#endif

		GAME_endOfTurn();
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
	int randVal;
	int dist[gridSize][gridSize];
	int myDist, otherDist;
	directions dir;
	player other = players[(nowPlaying + 1) % 2];

	TIMER_disable(TIMER_0);

	if(players[nowPlaying]->remainingWalls != 0) {
		myDist = AI_findDistFromArrival(dist, players[nowPlaying], other) - 2;

		otherDist = AI_findDistFromArrival(dist, other, players[nowPlaying]) - 2;

		randVal = AI_random(myDist * 3 + otherDist * 2 - 1);

		if(randVal < (otherDist * 2)) {
			AI_moveToken();
			return;
		}
		AI_placeWall();
	} else
		AI_moveToken();

	for(dir = (directions) 0; dir < DIR_none; dir++) other->availableMovement[dir] = 0;
}


bool AI_wallIsCorrect() {
	if(GAME_tmpWallOverlaps()) return false;
	if(!GAME_checkReachability(players[0], players[1])) return false;
	if(!GAME_checkReachability(players[1], players[0])) return false;

	return true;
}


#ifdef useFirstAlgorithm
void AI_moveToken() {
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


void AI_placeWall() {
	int totWeight = 0;
	int weights[gridSize - 1];
	int randVal;
	int row, col;
	bool horiz;

	for(row = 0; row < gridSize - 1; row++) weights[row] = randomMaxWallWeight - abs(players[nowPlaying]->finalR - row);

	for(row = 0; row < gridSize - 1; row++) {
		tmpWall.centerR = row;
		for(col = 0; col < gridSize - 1; col++) {
			tmpWall.centerC = col;
			for(horiz = false; horiz < 2; horiz++) {
				tmpWall.horiz = horiz;
				if(AI_wallIsCorrect()) totWeight += weights[row];
			}
		}
	}

	randVal = AI_random(totWeight - 1);

	for(row = 0; row < gridSize - 1; row++) {
		tmpWall.centerR = row;
		for(col = 0; col < gridSize - 1; col++) {
			tmpWall.centerC = col;
			for(horiz = false; horiz < 2; horiz++) {
				tmpWall.horiz = horiz;
				if(AI_wallIsCorrect()) {
					if(randVal < weights[row]) {
						isInsertingWall = true;
						return;
					}
					randVal -= weights[row];
				}
			}
		}
	}
}
#endif


#ifdef useSecondAlgorithm
void AI_moveToken() {
	int dist[gridSize][gridSize];
	int row, col, minDist;
	player p = players[nowPlaying];
	player_t tmpPlayer;

	minDist = AI_findDistFromArrival(dist, p, players[(nowPlaying + 1) % 2]);
	row = p->finalR;

	// find the arrival cell
	for(col = 0; col < gridSize; col++)
		if(dist[row][col] == minDist) break;

	// follow the backwards path: minDist == 1 is the current player pos (which is not evaluated by the cycle)
	for(minDist--; minDist != 1; minDist--) {
		tmpPlayer.r = row;
		tmpPlayer.c = col;
		if(--row != -1 && dist[row][col] == minDist && GAME_findMovementDir(&tmpPlayer, DIR_up, &tmpPlayer)) continue;                   // check up
		if(++row != -1 && --col != -1 && dist[row][col] == minDist && GAME_findMovementDir(&tmpPlayer, DIR_left, &tmpPlayer)) continue;  // check left
		if(++col != -1 && ++row != gridSize && dist[row][col] == minDist && GAME_findMovementDir(&tmpPlayer, DIR_down, &tmpPlayer))
			continue;  // check down
		if(--row != -1 && ++col != gridSize && dist[row][col] == minDist && GAME_findMovementDir(&tmpPlayer, DIR_right, &tmpPlayer))
			continue;  // check right
	}

	GAME_move(DIR_computeFromPoints(p->r, p->c, row, col));
}


void AI_placeWall() {
	int dist[gridSize][gridSize];
	int row, col, bestRow, bestCol;
	int newScore, maxScore = 0;
	int myDist, otherDist;
	int otherDeltaDist;
	bool horiz, bestHoriz;
	player other = players[(nowPlaying + 1) % 2];
	player me = players[nowPlaying];

	myDist = AI_findDistFromArrival(dist, me, other);
	otherDist = AI_findDistFromArrival(dist, other, me);

	for(row = 0; row < gridSize - 1; row++) {
		tmpWall.centerR = row;
		for(col = 0; col < gridSize - 1; col++) {
			tmpWall.centerC = col;
			for(horiz = false; horiz < 2; horiz++) {
				tmpWall.horiz = horiz;
				if(AI_wallIsCorrect()) {
					insertedWalls[numInsertedWalls++] = tmpWall;
					otherDeltaDist = AI_findDistFromArrival(dist, other, me) - otherDist;
					newScore = otherDeltaDist * 4 - 5 * (AI_findDistFromArrival(dist, me, other) - myDist);
					if(newScore > maxScore || (newScore == maxScore && AI_random(randomWeightOfReplacingEquallyGoodWall) == 0)) {
						maxScore = newScore;
						bestRow = row;
						bestCol = col;
						bestHoriz = horiz;
					}
					numInsertedWalls--;
				}
			}
		}
	}


	if(maxScore <= 0) {
		AI_moveToken();
		return;
	}

	isInsertingWall = true;
	tmpWall.centerR = bestRow;
	tmpWall.centerC = bestCol;
	tmpWall.horiz = bestHoriz;
}


int AI_findDistFromArrival(int dist[gridSize][gridSize], player p, player other) {
	int r, c;
	player_t tmpPlayer;
	int curDist = 0;
	bool pathFound = false;

	GAME_findMovements(p, other, false);

	for(r = 0; r < gridSize; r++)
		for(c = 0; c < gridSize; c++) dist[r][c] = 0;

	r = p->r;
	c = p->c;

	dist[r][c] = 1;

	if(p->availableMovement[DIR_up]) dist[r - 1][c] = 2;
	if(p->availableMovement[DIR_down]) dist[r + 1][c] = 2;
	if(p->availableMovement[DIR_left]) dist[r][c - 1] = 2;
	if(p->availableMovement[DIR_right]) dist[r][c + 1] = 2;
	if(p->availableMovement[DIR_up_left]) dist[r - 1][c - 1] = 2;
	if(p->availableMovement[DIR_up_right]) dist[r - 1][c + 1] = 2;
	if(p->availableMovement[DIR_down_left]) dist[r + 1][c - 1] = 2;
	if(p->availableMovement[DIR_down_right]) dist[r + 1][c + 1] = 2;

	for(curDist = 2; !pathFound; curDist++) {
		for(r = 0; r < gridSize && !pathFound; r++) {
			for(c = 0; c < gridSize && !pathFound; c++) {
				if(dist[r][c] == curDist) {
					if(r == p->finalR)
						pathFound = true;
					else {
						tmpPlayer.r = r;
						tmpPlayer.c = c;

						if(dist[r - 1][c] == 0 && GAME_findMovementDir(&tmpPlayer, DIR_up, &tmpPlayer)) dist[r - 1][c] = curDist + 1;
						if(dist[r + 1][c] == 0 && GAME_findMovementDir(&tmpPlayer, DIR_down, &tmpPlayer)) dist[r + 1][c] = curDist + 1;
						if(dist[r][c - 1] == 0 && GAME_findMovementDir(&tmpPlayer, DIR_left, &tmpPlayer)) dist[r][c - 1] = curDist + 1;
						if(dist[r][c + 1] == 0 && GAME_findMovementDir(&tmpPlayer, DIR_right, &tmpPlayer)) dist[r][c + 1] = curDist + 1;
					}
				}
			}
		}
	}

	return curDist - 1;
}
#endif
