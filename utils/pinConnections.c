#include "pinConnections.h"


void PINS_choose(pins pin, int value) {
	if(pin <= P0d15) {
		LPC_PINCON->PINSEL0 &= ~(3 << (pin * 2));
		LPC_PINCON->PINSEL0 |= (value << (pin * 2));
	} else if(pin <= P0d31_reserved) {
		LPC_PINCON->PINSEL1 &= ~(3 << ((pin - P0d16) * 2));
		LPC_PINCON->PINSEL1 |= (value << ((pin - P0d16) * 2));
	} else if(pin <= P1d15) {
		LPC_PINCON->PINSEL2 &= ~(3 << ((pin - P1d0) * 2));
		LPC_PINCON->PINSEL2 |= (value << ((pin - P1d0) * 2));
	} else if(pin <= P1d31) {
		LPC_PINCON->PINSEL3 &= ~(3 << ((pin - P1d16) * 2));
		LPC_PINCON->PINSEL3 |= (value << ((pin - P1d16) * 2));
	} else {
		LPC_PINCON->PINSEL4 &= ~(3 << ((pin - P2d0) * 2));
		LPC_PINCON->PINSEL4 |= (value << ((pin - P2d0) * 2));
	}
}


void PINS_setAsInput(pins pin) {
	if(pin <= P0d31_reserved)
		LPC_GPIO0->FIODIR &= ~(1 << pin);
	else if(pin <= P1d31)
		LPC_GPIO1->FIODIR &= ~(1 << (pin - P1d0));
	else if(pin <= P2d15_reserved)
		LPC_GPIO2->FIODIR &= ~(1 << (pin - P2d0));
}


void PINS_setAsOutput(pins pin) {
	if(pin <= P0d31_reserved)
		LPC_GPIO0->FIODIR |= (1 << pin);
	else if(pin <= P1d31)
		LPC_GPIO1->FIODIR |= (1 << (pin - P1d0));
	else if(pin <= P2d15_reserved)
		LPC_GPIO2->FIODIR |= (1 << (pin - P2d0));
}

bool PINS_readGPIOvalue(pins pin) {
	int tmp;
	if(pin <= P0d31_reserved)
		tmp = LPC_GPIO0->FIOPIN & (1 << pin);
	else if(pin <= P1d31)
		tmp = LPC_GPIO1->FIOPIN & (1 << (pin - P1d0));
	else if(pin <= P2d15_reserved)
		tmp = LPC_GPIO2->FIOPIN & (1 << (pin - P2d0));
	return (bool) (tmp != 0);
}
