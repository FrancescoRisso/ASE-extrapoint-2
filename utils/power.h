#ifndef __POWER_H
#define __POWER_H

#include "bool.h"
#include "lpc17xx.h"

typedef enum {
	POW_reserved0,
	POW_timer0,
	POW_timer1,
	POW_uart0,
	POW_uart1,
	POW_reserved1,
	POW_pwm1,
	POW_i2c0,
	POW_spi,
	POW_realTimeClock,
	POW_ssp1,
	POW_reserved2,
	POW_adc,
	POW_can1,
	POW_can2,
	POW_gpio,
	POW_repetitiveInterruptTimer,
	POW_motor,
	POW_qei,
	POW_i2c1,
	POW_reserved3,
	POW_ssp0,
	POW_timer2,
	POW_timer3,
	POW_uart2,
	POW_uart3,
	POW_i2c2,
	POW_i2s,
	POW_reserved4,
	POW_gpdma,
	POW_ethernet,
	POW_usb
} POW_peripheral;


typedef enum { POW_awake, POW_sleep, POW_deepSleep } POW_mode;


/*
	POW_setPeripheralOn
	---------------------------------------------------------------------
	Turns on a peripheral
	---------------------------------------------------------------------
	PARAMETERS:
		- target: the peripheral to be turned on
*/
void POW_setPeripheralOn(POW_peripheral target);


/*
	POW_setPeripheralOff
	---------------------------------------------------------------------
	Turns off a peripheral
	---------------------------------------------------------------------
	PARAMETERS:
		- target: the peripheral to be turned off
*/
void POW_setPeripheralOff(POW_peripheral target);


/*
	POW_setPowerMode
	---------------------------------------------------------------------
	Puts the system in a certain power consumption mode
	---------------------------------------------------------------------
	PARAMETERS:
		- mode: the desired target mode
*/
void POW_setPowerMode(POW_mode mode);


/*
	POW_isPeripheralOn
	---------------------------------------------------------------------
	Tells if a peripheral is on or off
	---------------------------------------------------------------------
	PARAMETERS:
		- target: the peripheral to be turned off
	---------------------------------------------------------------------
	OUTPUT:
		- true: the peripheral is on
		- false: the peripheral is off
*/
bool POW_isPeripheralOn(POW_peripheral target);

#endif
