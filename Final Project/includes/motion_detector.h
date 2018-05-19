#ifndef _motion_detector_h_
#define _motion_detector_h_

#include "common.h"

#define motionDetected (~PINA & 0x08) // use pin 3 for motion detector

enum MOTION_SENSED_STATES {NO_MOTION_SENSED, MOTION_SENSED} motionSensed; // 0 - off / 1 - on

enum MOTION_STATES {SM_MOTION_START, SM_MOTION_INIT, SM_MOTION_SENSED, SM_NO_MOTION_SENSED};

State motionTckFct(State state) {
	switch (state) {							// start transitions
		case SM_MOTION_START:
		state = SM_MOTION_INIT;
		break;
		case SM_MOTION_INIT:
		state = SM_NO_MOTION_SENSED;
		motionSensed = NO_MOTION_SENSED;
		break;
		case SM_NO_MOTION_SENSED:
		if (motionDetected) {
			state = SM_MOTION_SENSED;
			motionSensed = MOTION_SENSED;
		}
		break;
		case SM_MOTION_SENSED:
		if (!motionDetected) {
			state = SM_NO_MOTION_SENSED;
			motionSensed = NO_MOTION_SENSED;
		}
		break;
		default:
		state = SM_MOTION_START;
		break;
	}												// end transitions
	
	switch (state) {								// start actions
		case SM_MOTION_START: break;
		case SM_MOTION_INIT: break;
		case SM_NO_MOTION_SENSED: break;
		case SM_MOTION_SENSED: break;
	}												// end actions
	return state;
}

#endif
