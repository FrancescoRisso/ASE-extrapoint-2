#include "game.h"
#include "internal-game-functions.h"

extern player players[2];
extern wallPos insertedWalls[16];
extern int numInsertedWalls;
extern bool isFirstGame;
extern int nowPlaying;

// TODO: Remove in real code
void fakeGame() {
	int i;
	if(isFirstGame) {
		players[0]->remainingWalls = 7;
		players[1]->remainingWalls = 1;

		insertedWalls[numInsertedWalls].centerC = 0;
		insertedWalls[numInsertedWalls].centerR = 3;
		insertedWalls[numInsertedWalls++].horiz = true;

		insertedWalls[numInsertedWalls].centerC = 0;
		insertedWalls[numInsertedWalls].centerR = 5;
		insertedWalls[numInsertedWalls++].horiz = true;

		insertedWalls[numInsertedWalls].centerC = 1;
		insertedWalls[numInsertedWalls].centerR = 4;
		insertedWalls[numInsertedWalls++].horiz = false;

		insertedWalls[numInsertedWalls].centerC = 1;
		insertedWalls[numInsertedWalls].centerR = 0;
		insertedWalls[numInsertedWalls++].horiz = false;

		insertedWalls[numInsertedWalls].centerC = 2;
		insertedWalls[numInsertedWalls].centerR = 0;
		insertedWalls[numInsertedWalls++].horiz = true;

		insertedWalls[numInsertedWalls].centerC = 2;
		insertedWalls[numInsertedWalls].centerR = 3;
		insertedWalls[numInsertedWalls++].horiz = true;

		insertedWalls[numInsertedWalls].centerC = 3;
		insertedWalls[numInsertedWalls].centerR = 4;
		insertedWalls[numInsertedWalls++].horiz = false;

		insertedWalls[numInsertedWalls].centerC = 5;
		insertedWalls[numInsertedWalls].centerR = 4;
		insertedWalls[numInsertedWalls++].horiz = false;

		players[0]->r = 2;
		players[1]->r = 3;

		numInsertedWalls = 8;

		nowPlaying++;

		for(i = 0; i < 8; i++) GAME_drawWall(insertedWalls[i], Black);
	}
}
