#ifndef _input_sm_h_
#define _input_sm_h_

#include "common.h"
#include "globalData.h"

#define jump (~PINA & 0x02)
#define reset (~PINA & 0x04)

// for potentiometer that the joystick uses
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

// Tested with joystick
// Max value reached was 1011
const unsigned short neutral = 547;	// this should be the default position of the joystick,
									// if it's not moved from the neutral positions upon startup
ConstByte tolerance = 50; // allow some wiggle room with the neutral position (<10%)

unsigned short joyStickPeriod = 50; 

/* State machine to move player left and right
 * moveDirection is a global variable that will be used to update the player position on the display
 */
enum MOVE_DIRECTION {MOVE_STOP, MOVE_LEFT, MOVE_RIGHT} moveDirection; // Global. move the player left or right based on joystick input

enum DIRECTION_STATES {SM_DIRECT_START, SM_DIRECT_INIT, SM_DIRECT_WAIT};

State joystickTckFct(State state) {
	unsigned short current = ADC;		// get the current value of the joystick position
	switch (state) {					// begin transitions
		case SM_DIRECT_START:
			state = SM_DIRECT_INIT;
			break;
		case SM_DIRECT_INIT:
			state = SM_DIRECT_WAIT;
			break;
		case SM_DIRECT_WAIT:
			if (current < (neutral - tolerance) ) {
				moveDirection = MOVE_LEFT;
			}
			else if ( current > (neutral + tolerance) ) {
				moveDirection = MOVE_RIGHT;
			}
			else {
				moveDirection = MOVE_STOP;
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
 */

ConstByte buttonPeriod = 50;

enum BUTTON_STATES {SM_BUTTON_START, SM_BUTTON_INIT, SM_BUTTON_WAIT, SM_BUTTON_RESET, SM_BUTTON_RELEASE};

State buttonTckFct(State state) {
	static Byte ticks;
	switch (state) {					// begin transitions
		case SM_BUTTON_START:
			state = SM_BUTTON_INIT;
			break;
		case SM_BUTTON_INIT:			// initialize with player grounded and ready to attack
			state = SM_BUTTON_WAIT;
			gameReset = false;
			break;
		case SM_BUTTON_WAIT:			
			if (jump && !reset) {
				isJumping = true;
				state = SM_BUTTON_RELEASE;
			}
			if (reset) {
				state = SM_BUTTON_RESET;
				gameOver = false;
				gameReset = true;
			}
			break;
		case SM_BUTTON_RESET:
			if (ticks >= 200) {
				state = SM_BUTTON_RELEASE;
				gameReset = false;
			}
			else {
				ticks += buttonPeriod;
			}
			break;
		case SM_BUTTON_RELEASE:
			if (!jump && !reset) {
				state = SM_BUTTON_WAIT;
				
			}
			break;
		default:
			state = SM_BUTTON_START;
			break;
	}									// end transitions
	return state;
}

#endif