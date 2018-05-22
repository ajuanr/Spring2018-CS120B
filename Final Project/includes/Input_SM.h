#ifndef _input_sm_h_
#define _input_sm_h_

#include "common.h"
#include "globalData.h"

#define jump (~PINA & 0x02)
#define reset (~PINA & 0x04)
#define attack (~PINA & 0x08)

// for potentiometer that the joystick uses
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

// Tested with joystick
// Max value reached was 1011
const unsigned short neutral = 536;	// this should be the default position of the joystick,
									// if it's not moved from the neutral positions upon startup
ConstByte tolerance = 75; // allow some wiggle room with the neutral position (<10%)

ConstByte INPUT_PERIOD = 50; 

/* State machine to move player left and right
 * moveDirection is a global variable that will be used to update the player position on the display
 */
enum MOVE_DIRECTION {MOVE_STOP, MOVE_LEFT, MOVE_RIGHT} moveDirection; // Global. move the player left or right based on joystick input

enum DIRECTION_STATES {SM_DIRECT_START, SM_DIRECT_INIT, SM_DIRECT_WAIT};

State directionTckFct(State state) {
	unsigned short current = ADC;		// get the current value of the joystick position
	switch (state) {					// begin transitions
		case SM_DIRECT_START:
		state = SM_DIRECT_INIT;
		break;
		case SM_DIRECT_INIT:
		state = SM_DIRECT_WAIT;
		break;
		case SM_DIRECT_WAIT:
		moveDirection = MOVE_STOP;  // assume joystick is in neutral position
		// adjust if necessary
		if (current < (neutral - tolerance) ) {
			moveDirection = MOVE_LEFT;
		}
		else if ( current > (neutral + tolerance) ) {
			moveDirection = MOVE_RIGHT;
		}
		break;
		default:
		state = SM_DIRECT_START;
		break;
	}									// end transitions
	
	switch (state) {					// start actions
		case SM_DIRECT_START: break;
		case SM_DIRECT_INIT: break;
		case SM_DIRECT_WAIT: break;
	}									// end actions
	return state;
}

/* State machine to make player jump or attack
 * isJumping and attackState are a global variables that will be used to update the player/attack position on the display
 * they are reset back to default values after player either lands, or is ready to attack again
 * the resets are handled in another state machine
 */
enum BUTTON_STATES {SM_BUTTON_START, SM_BUTTON_INIT, SM_BUTTON_WAIT, SM_BUTTON_RELEASE};

State buttonTckFct(State state) {
	switch (state) {					// begin transitions
		case SM_BUTTON_START:
		state = SM_BUTTON_INIT;
		break;
		case SM_BUTTON_INIT:			// initialize with player grounded and ready to attack
		state = SM_BUTTON_WAIT;
		isJumping = false;
		gameReset = false;
		attackState = ATTACK_READY;
		break;
		case SM_BUTTON_WAIT:
		if (jump || attack || reset) {
			state = SM_BUTTON_RELEASE;
			if (jump && !attack && !reset) {
				isJumping = true;
			}
			if (attack && !jump && !reset) {
				attackState = ATTACK_NOT_READY;
			}
			if (reset) {
				gameReset = true; // resets to false in TIMERISR
			}
		}
		break;
		case SM_BUTTON_RELEASE:
		if (!jump && !attack && !reset) {
			state = SM_BUTTON_WAIT;
		}
		break;
		default:
		state = SM_BUTTON_START;
	}									// end transitions
	return state;
}

#endif