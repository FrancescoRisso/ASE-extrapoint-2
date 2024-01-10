#include "power.h"

void POW_setPeripheralOn(POW_peripheral target) {
	LPC_SC->PCONP |= (1 << target);
}


void POW_setPeripheralOff(POW_peripheral target) {
	LPC_SC->PCONP &= ~(1 << target);
}


void POW_setPowerMode(POW_mode mode) {
	switch(mode) {
		case POW_awake:  // set PCON = ...00
			LPC_SC->PCON &= ~3;
			break;
		case POW_sleep:  // set PCON = ...01
			LPC_SC->PCON |= 1;
			LPC_SC->PCON &= ~2;
			break;
		case POW_deepSleep:  // set PCON = ...11
			LPC_SC->PCON |= 3;
			break;
	}
}


bool POW_isPeripheralOn(POW_peripheral target) {
	return (bool) ((LPC_SC->PCONP & (1 << target)) != 0);
}
