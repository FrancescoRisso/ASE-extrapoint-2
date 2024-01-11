#include "AI.h"


int AI_random(int max) {
	int i, sum = 0;

	for(i = 0; i < randomIterations; i++) sum += LPC_RIT->RICOUNTER % max;
	return sum % max;
}


void AI_move(int timeLeft) {
	int move = lastMove;
	decodeMove();

	if(timeLeft == 1 || AI_random(1)) {
		if(isValid) {
			// mirror move
		} else {
			// do random move
		}

		GAME_endOfTurn();
	};
}
