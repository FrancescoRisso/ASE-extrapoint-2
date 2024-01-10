#include "joystick.h"

pins pinConnection[5] = {P1d29, P1d26, P1d27, P1d28, P1d25};
JOY_status status[5] = {JOY_release, JOY_release, JOY_release, JOY_release, JOY_release};


JOY_status JOY_updateStatus(JOY_direction dir) {
	if(!PINS_readGPIOvalue(pinConnection[dir])) {
		switch(status[dir]) {
			case JOY_release: status[dir] = JOY_press; break;
			case JOY_press: status[dir] = JOY_longPress; break;
			default: break;
		}
	} else if(status[dir] != JOY_release) {
		status[dir] = JOY_release;
	}
	return status[dir];
}

void JOY_init(void) {
	int dir;
	for(dir = 0; dir < 5; dir++) {
		PINS_choose(pinConnection[dir], 0);
		PINS_setAsInput(pinConnection[dir]);
	}
}
