#include "buttons.h"

extern bool isFirstGame;

extern gameStatuses gameStatus;

// insert your handlers here
void myINT0handler() {
	switch(gameStatus) {
		case GAME_chooseNumBoards: break;
		case GAME_AIorPlayer: break;
		case GAME_game: break;
	}
}

void myKEY1handler() {
	GAME_switchPlayerWall();
}

void myKEY2handler() {
	GAME_rotateWall();
}

// do not edit below
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
#include "../RIT/RIT.h"

int press_count = 0;
#endif

pins BTN_pinConnection[3] = {P2d10, P2d11, P2d12};
IRQn_Type interrupts[3] = {EINT0_IRQn, EINT1_IRQn, EINT2_IRQn};

BTN_status BTN_currentStatus[3] = {BTN_release, BTN_release, BTN_release};

#ifndef _BUTTONS_DEBOUNCE

void EINT0_IRQHandler(void) {
	myINT0handler();
	myNVIC_clearInterrupt(myNVIC_buttonINT0);
}

void EINT1_IRQHandler(void) {
	myKEY1handler();
	myNVIC_clearInterrupt(myNVIC_buttonKEY1);
}

void EINT2_IRQHandler(void) {
	myKEY2handler();
	myNVIC_clearInterrupt(myNVIC_buttonKEY2);
}

#else

void btnEnableRIT() {
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
	press_count++;
	if(press_count == 1) {
		init_RIT(TIME_50MS);
		enable_RIT();
	}
#endif
}

void btnDisableRIT() {
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
	press_count--;
	if(press_count == 0) { disable_RIT(); }
#endif
}

void EINT0_IRQHandler(void) {
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
	// set pin as GPIO to be able to read it
	PINS_choose(BTN_pinConnection[0], 0);
#endif
	btnEnableRIT();
	myNVIC_clearInterrupt(myNVIC_buttonINT0);
}

void EINT1_IRQHandler(void) {
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
	// set pin as GPIO to be able to read it
	PINS_choose(BTN_pinConnection[1], 0);
#endif
	btnEnableRIT();
	myNVIC_clearInterrupt(myNVIC_buttonKEY1);
}

void EINT2_IRQHandler(void) {
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
	// set pin as GPIO to be able to read it
	PINS_choose(BTN_pinConnection[2], 0);
#endif
	btnEnableRIT();
	myNVIC_clearInterrupt(myNVIC_buttonKEY2);
}

#endif


void BTN_enable(button btn, unsigned int priority, BTN_sensitivity sensitivity, bool enableInterrupt) {
	PINS_choose(BTN_pinConnection[btn], 1);
	PINS_setAsInput(BTN_pinConnection[btn]);

	LPC_SC->EXTMODE &= ~(1 << btn);
	LPC_SC->EXTMODE |= (sensitivity << btn);

	if(enableInterrupt) {
		NVIC_SetPriority(interrupts[btn], priority);
		NVIC_EnableIRQ(interrupts[btn]);
	}
}

#ifdef _BUTTONS_DEBOUNCE
void callDebouncedHandler(button btn) {
	switch(btn) {
		case BTN_int0: myINT0handler(); break;

		case BTN_key1: myKEY1handler(); break;

		case BTN_key2: myKEY2handler(); break;

		default: break;
	}
}
#endif

#ifdef _BUTTONS_DEBOUNCE
BTN_status BTN_updateStatus(button btn) {
	if(!PINS_readGPIOvalue(BTN_pinConnection[btn])) {
		switch(BTN_currentStatus[btn]) {
			case BTN_release: BTN_currentStatus[btn] = BTN_press;
#ifdef _BUTTONS_DEBOUNCE
				callDebouncedHandler(btn);
#endif
				break;
			case BTN_press: BTN_currentStatus[btn] = BTN_longPress; break;
			default: break;
		}
	} else if(BTN_currentStatus[btn] != BTN_release) {
		BTN_currentStatus[btn] = BTN_release;
#ifdef _BUTTONS_ENABLE_RIT_ON_PRESS
		btnDisableRIT();

		// re-set pin as interrupt-receiving
		PINS_choose(BTN_pinConnection[btn], 1);
#endif
	}
	return BTN_currentStatus[btn];
}

void BTN_updateStatusAll() {
	BTN_updateStatus(BTN_int0);
	BTN_updateStatus(BTN_key1);
	BTN_updateStatus(BTN_key2);
}
#endif
