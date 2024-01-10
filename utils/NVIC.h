#ifndef __myNVIC_H
#define __myNVIC_H

#include "lpc17xx.h"

typedef enum {
	todo_myNVIC_watchdog,
	myNVIC_timer0,
	myNVIC_timer1,
	myNVIC_timer2,
	myNVIC_timer3,
	todo_myNVIC_uart0,
	todo_myNVIC_uart1,
	todo_myNVIC_uart2,
	todo_myNVIC_uart3,
	todo_myNVIC_pwm1,
	todo_myNVIC_i2c0,
	todo_myNVIC_i2c1,
	todo_myNVIC_i2c2,
	todo_myNVIC_ssp0,
	todo_myNVIC_ssp1,
	todo_myNVIC_pll0lock,
	todo_myNVIC_realTimeClock,
	myNVIC_buttonINT0,
	myNVIC_buttonKEY1,
	myNVIC_buttonKEY2,
	myNVIC_eint3,
	todo_myNVIC_analogDigitalConverter,
	todo_myNVIC_brownOut,
	todo_myNVIC_usb,
	todo_myNVIC_can,
	todo_myNVIC_dma,
	todo_myNVIC_i2s,
	todo_myNVIC_ethernet,
	myNVIC_rit,
	todo_myNVIC_motor,
	todo_myNVIC_qei,
	todo_myNVIC_pll1lock,
	todo_myNVIC_usbActivity,
	todo_myNVIC_canActivity
} interrupt;


/*
	myNVIC_clearInterrupt
	---------------------------------------------------------------------
	Clears the bit of an interrupt
	---------------------------------------------------------------------
	PARAMETERS:
		- i: the id of the interrupt to be cleared
*/
void myNVIC_clearInterrupt(interrupt i);


#endif
