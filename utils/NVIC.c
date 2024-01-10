#include "NVIC.h"


void myNVIC_clearInterrupt(interrupt i) {
	switch(i) {
		case myNVIC_timer0: LPC_TIM0->IR = 1; break;
		case myNVIC_timer1: LPC_TIM1->IR = 1; break;
		case myNVIC_timer2: LPC_TIM2->IR = 1; break;
		case myNVIC_timer3: LPC_TIM3->IR = 1; break;

		case myNVIC_buttonINT0: LPC_SC->EXTINT &= 1; break;
		case myNVIC_buttonKEY1: LPC_SC->EXTINT &= (1 << 1); break;
		case myNVIC_buttonKEY2: LPC_SC->EXTINT &= (1 << 2); break;
		case myNVIC_eint3: LPC_SC->EXTINT &= (1 << 3); break;

		case myNVIC_rit: LPC_RIT->RICTRL |= 1; break;

		default: break;
	}
}
