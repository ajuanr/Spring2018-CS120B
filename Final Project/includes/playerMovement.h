/* this SM controls the postion of the player on the screeen
 * player only moves left and right
 */

#ifndef _playerMovement_h_
#define _playerMovement_h_

#include "common.h"

Byte playerPos; // add 16 to get actually position on row 2 of LCD
Byte LCDpos;	// want to display player on second row of LCD
ConstByte lvlWidth = 32;
ConstByte LCDwidth = 16;

enum POS_STATES {SM_POS_START, SM_POS_INIT, SM_POS_WAIT, SM_POS};

State posTckFct(State state) {
	static Byte dir;
	switch (state) {						// start transitions
		case SM_POS_START:
		state = SM_POS_INIT;
		break;
		case SM_POS_INIT:
		state = SM_POS_WAIT;
		playerPos = 1;
		LCDpos = playerPos + LCDwidth;
		dir = MOVE_RIGHT; // pos = right / 0 = left
		break;
		case SM_POS_WAIT:
		if (moveDirection != MOVE_STOP) { // player is moving
			state = SM_POS;
		}
		break;
		case SM_POS:
		if (moveDirection == MOVE_STOP) {
			state = SM_POS_WAIT;
		}
		break;
		default:
		state = SM_POS_START;
		break;
	}											// end transitions
	
	switch (state) {							// start actions
		case SM_POS_START: break;
		case SM_POS_INIT: break;
		case SM_POS_WAIT: break;
		case SM_POS:
		dir = (moveDirection == MOVE_RIGHT) ? MOVE_RIGHT : MOVE_LEFT;
		if (dir == MOVE_RIGHT && playerPos < lvlWidth) { // MIGHT HAVE OFF BY ONE ERROR
			++playerPos;
		}
		else if (dir == MOVE_LEFT && playerPos > 1) {
			--playerPos;
		}
		if (jumpState == ON_GROUND) {	// only force LCD position to second row
										// when player is not jumping
			LCDpos = (playerPos % LCDwidth) + LCDwidth;
			if (LCDpos <= LCDwidth) {	// handle bug when right edge of screen is reached
				LCDpos += LCDwidth;
			}
		}
		else if (jumpState == IN_AIR) {
			if (dir == MOVE_RIGHT && playerPos < lvlWidth) { // MIGHT HAVE OFF BY ONE ERROR
				++LCDpos;
			}
			else if (dir == MOVE_LEFT && playerPos > 1) {
				--LCDpos;
			}
			//LCDpos = playerPos % LCDwidth;
		}

		break;
	}											// end actions
	return state;
}

enum LCD_JUMP_STATES {SM_JUMP_START, SM_JUMP_INIT, SM_JUMP_ON_GROUND, SM_JUMP_IN_AIR};


State jumpTckFct(State state) {
	ConstByte jumpTicks = 20; // if period is 50 ms
	static Byte ticks;
	switch (state) {
		case SM_JUMP_START:
			state = SM_JUMP_INIT;
			break;
		case SM_JUMP_INIT:
			state = SM_JUMP_ON_GROUND;
			jumpState = ON_GROUND;
			ticks = 0;
			break;
		case SM_JUMP_ON_GROUND:
			if (jumpState == IN_AIR) {
				state = SM_JUMP_IN_AIR;
				LCDpos -= LCDwidth;

				ticks = 0;
			}
			break;
		case SM_JUMP_IN_AIR:
			if (ticks >= jumpTicks) {
				state = SM_JUMP_ON_GROUND;
				jumpState = ON_GROUND;
				LCDpos += LCDwidth;
			}
			++ticks;
			break;
		default:
			state = SM_POS_START;
	}											// end transitions
	return state;
}

#endif
