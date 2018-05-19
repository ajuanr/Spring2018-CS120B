/* this SM controls the postion of the player on the screeen
 * player only moves left and right
 */

#ifndef _playerMovement_h_
#define _playerMovement_h_

#include "common.h"

Byte playerPos; // add 16 to get actually position on row 2 of LCD
ConstByte lvlWidth = 32;

enum POS_STATES {SM_POS_START, SM_POS_INIT, SM_POS_WAIT, SM_POS};

State posTckFct(State state) {
	static Byte dir;
	switch (state) {						// start transtitions
		case SM_POS_START:
		state = SM_POS_INIT;
		break;
		case SM_POS_INIT:
		state = SM_POS_WAIT;
		playerPos = 0;
		dir = MOVE_RIGHT; // pos = right / 0 = left
		break;
		case SM_POS_WAIT:
		if (moveDirection != MOVE_STOP) { // player is moving
			state = SM_POS;
			//dir = (moveDirection == MOVE_RIGHT) ? MOVE_RIGHT : MOVE_LEFT;
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
		else if (dir == MOVE_LEFT && playerPos > 0) {
			--playerPos;
		}
		break;
	}											// end actions
	return state;
}

#endif
