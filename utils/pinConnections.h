#ifndef __PINS_H
#define __PINS_H

#include "bool.h"
#include "lpc17xx.h"


typedef enum {
	P0d0,
	P0d1,
	P0d2,
	P0d3,
	P0d4,
	P0d5,
	P0d6,
	P0d7,
	P0d8,
	P0d9,
	P0d10,
	P0d11,
	P0d12_reserved,
	P0d13_reserved,
	P0d14_reserved,
	P0d15,
	P0d16,
	P0d17,
	P0d18,
	P0d19,
	P0d20,
	P0d21,
	P0d22,
	P0d23,
	P0d24,
	P0d25,
	P0d26,
	P0d27,
	P0d28,
	P0d29,
	P0d30,
	P0d31_reserved,
	P1d0,
	P1d1,
	P1d2,
	P1d3,
	P1d4,
	P1d5,
	P1d6,
	P1d7,
	P1d8,
	P1d9,
	P1d10,
	P1d11,
	P1d12,
	P1d13,
	P1d14,
	P1d15,
	P1d16,
	P1d17,
	P1d18,
	P1d19,
	P1d20,
	P1d21,
	P1d22,
	P1d23,
	P1d24,
	P1d25,
	P1d26,
	P1d27,
	P1d28,
	P1d29,
	P1d30,
	P1d31,
	P2d0,
	P2d1,
	P2d2,
	P2d3,
	P2d4,
	P2d5,
	P2d6,
	P2d7,
	P2d8,
	P2d9,
	P2d10,
	P2d11,
	P2d12,
	P2d13,
	P2d14_reserved,
	P2d15_reserved
} pins;

/*
	PINS_choose
	---------------------------------------------------------------------
	Sets the value for the PINSELn registers
	---------------------------------------------------------------------
	PARAMETERS:
		- pin: the pin to be updated, in the form Pxdy, where x and y are
			the name of the pin in Px.y (eg P0.26 -> P0d26)
		- value: the value to be set (within 0 and 3)
*/
void PINS_choose(pins pin, int value);


/*
	PIN_setAsInput
	---------------------------------------------------------------------
	Sets a GPIO pin to be an input
	---------------------------------------------------------------------
	PARAMETERS:
		- pin: the pin to be set
*/
void PINS_setAsInput(pins pin);


/*
	PIN_setAsOutput
	---------------------------------------------------------------------
	Sets a GPIO pin to be an output
	---------------------------------------------------------------------
	PARAMETERS:
		- pin: the pin to be set
*/
void PINS_setAsOutput(pins pin);


/*
	PINS_readGPIOvalue
	---------------------------------------------------------------------
	Reads the value of a pin, considering it as an input GPIO
	---------------------------------------------------------------------
	PARAMETERS:
		- pin: the pin the be read
	---------------------------------------------------------------------
	OUTPUT:
		- the (boolean) value
*/
bool PINS_readGPIOvalue(pins pin);

#endif
