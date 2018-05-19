#ifndef _playerActions_h_
#define _playerActions_h_

#include "common.h"

#define jump (~PINA & 0x02)
#define attack (~PINA & 0x04)

// for potentiometer that the joystick uses
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

// Based on testing with a 103 pot and an LCD screen
// Max value reached was 1008
// Neutral should be in middle 1008/2 = 504
const unsigned short neutral = 504;	// this should be the default position of the joystick,
									// if it's not moved from the neutral positions upon startup
const unsigned short tolerance = 100; // allow some wiggle room with the neutral position (10%)

const unsigned long INPUT_PERIOD = 50; 

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
		// adjust if neccessary
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
 * jumpState and attackState are a global variables that will be used to update the player/attack position on the display
 * they are reset back to default values after player either lands, or is ready to attack again
 * the resets are handled in another state machine
 */
enum JUMP_STATE {ON_GROUND, IN_AIR} jumpState; // set to IN_AIR when player presses jump button
enum ATTACK_STATE {ATTACK_READY, ATTACK_NOT_READY} attackState; // set to NOT_READY when player presses Attack button
// these two states are reset in another state machine

enum BUTTON_STATES {SM_BUTTON_START, SM_BUTTON_INIT, SM_BUTTON_WAIT, SM_BUTTON_RELEASE};

State buttonTckFct(State state) {
	switch (state) {					// begin transitions
		case SM_BUTTON_START:
		state = SM_BUTTON_INIT;
		break;
		case SM_BUTTON_INIT:			// initialize with player grounded and ready to attack
		state = SM_BUTTON_WAIT;
		jumpState = ON_GROUND;
		attackState = ATTACK_READY;
		break;
		case SM_BUTTON_WAIT:
		if (jump || attack) {
			state = SM_BUTTON_RELEASE;
			if (jump && !attack) {
				jumpState = IN_AIR;
			}
			if (attack && !jump) {
				attackState = ATTACK_NOT_READY;
			}
		}
		break;
		case SM_BUTTON_RELEASE:
		if (!jump && !attack) {
			state = SM_BUTTON_WAIT;
		}
		break;
		default:
		state = SM_BUTTON_START;
	}									// end transitions
	return state;
}

#endif