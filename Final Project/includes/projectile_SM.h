#ifndef _projectile_sm_h_
#define _projectile_sm_h_

#include "common.h"
#include "scheduler.h"
#include "shiftRegister.h"
#include "globalData.h"

const unsigned long PROJ_PERIOD = 250;

enum PROJECTILE {SM_PROJ_START, SM_PROJ_INIT, SM_PROJ_WAITFIRE, SM_PROJ_MOVING};

State projTckFct(State state) {
	static Byte i;
	static Byte output;
	switch (state) {		// transitions
		case SM_PROJ_START:
		state = SM_PROJ_INIT;
		break;
		case SM_PROJ_INIT:
		state = SM_PROJ_WAITFIRE;
		break;
		case SM_PROJ_WAITFIRE:
		if (i >= 8) {
			state = SM_PROJ_MOVING;
			i = 0;
		}
		break;
		case SM_PROJ_MOVING:
		if (projPos > playerPos + 16) {
			state = SM_PROJ_INIT;
			
		}
		default:
		state = SM_PROJ_START;
	}						// end transitions
	
	switch (state) {		// start actions
		case SM_PROJ_START: break;
		case SM_PROJ_INIT:
		i = 0;
		output = 0xFF;
		projPos = LCD_pos;
		break;
		case SM_PROJ_WAITFIRE:
		output >>= 1;
		++i;
		break;
		case SM_PROJ_MOVING:
		++projPos;
		break;
	}
	shiftDataIn(output);
	return state;
}

#endif