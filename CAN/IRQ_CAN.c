/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "../game/game.h"
#include "CAN.h"


extern int myID;
extern bool dualBoard;
extern bool otherPlayerReady;
extern bool ready;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler(void) {
	int msg;
	msg = CAN_rdMsg();        /* Read the message */
	LPC_CAN1->CMR = (1 << 2); /* Release receive buffer */

	if(myID == -1) {
		myID = 1;
		GAME_twoBoardGame(false);
		return;
	}

	if(!otherPlayerReady) {
		otherPlayerReady = true;
		if(ready) GAME_init();
		return;
	}

	GAME_execEncodedMove(msg);
}
