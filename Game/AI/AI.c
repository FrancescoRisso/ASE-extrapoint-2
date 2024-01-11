#include "AI.h"


int AI_random(int max) {
	int i, sum = 0;

	for(i = 0; i < randomIterations; i++) sum += LPC_RIT->RICOUNTER % max;
	return sum % max;
}
