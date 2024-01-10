#include "timers.h"

// define here your timer handlers
void myTimer0handler() {
	GAME_reduceTimer();
}

void myTimer1handler() {
	GAME_redrawCellsAfterText();
}

void myTimer2handler() {}

void myTimer3handler() {}


// please do not edit these
void TIMER0_IRQHandler(void) {
	myTimer0handler();
	myNVIC_clearInterrupt(myNVIC_timer0);
}

void TIMER1_IRQHandler(void) {
	myTimer1handler();
	myNVIC_clearInterrupt(myNVIC_timer1);
}

void TIMER2_IRQHandler(void) {
	myTimer2handler();
	myNVIC_clearInterrupt(myNVIC_timer2);
}

void TIMER3_IRQHandler(void) {
	myTimer3handler();
	myNVIC_clearInterrupt(myNVIC_timer3);
}


LPC_TIM_TypeDef* timers[4] = {LPC_TIM0, LPC_TIM1, LPC_TIM2, LPC_TIM3};
POW_peripheral powerIDs[4] = {POW_timer0, POW_timer1, POW_timer2, POW_timer3};
IRQn_Type interruptIDs[4] = {TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn};


void TIMER_enable(timer t) {
	POW_setPeripheralOn(powerIDs[t]);
	timers[t]->TCR = 1;
}


void TIMER_disable(timer t) {
	if(!POW_isPeripheralOn(powerIDs[t])) return;
	timers[t]->TCR = 0;
	POW_setPeripheralOff(powerIDs[t]);
}


void TIMER_enableReset(timer t, bool enable) {
	if(enable)
		timers[t]->TCR = timers[t]->TCR | 2;
	else
		timers[t]->TCR = timers[t]->TCR & ~2;
}


void TIMER_setValue(timer t, matchRegister mr, int value, timerCounterAction action) {
	LPC_TIM_TypeDef* timerStructPtr = timers[t];

	switch(mr) {
		case TIMER_matchReg0: timerStructPtr->MR0 = value; break;
		case TIMER_matchReg1: timerStructPtr->MR1 = value; break;
		case TIMER_matchReg2: timerStructPtr->MR2 = value; break;
		case TIMER_matchReg3: timerStructPtr->MR3 = value; break;
	}

	timerStructPtr->MCR &= ~(7 << (mr * 3));
	timerStructPtr->MCR |= (action << (mr * 3));

	NVIC_EnableIRQ(interruptIDs[t]);
}
