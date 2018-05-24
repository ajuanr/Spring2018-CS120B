/* this SM controls the postion of the player on the screeen
 * player only moves left and right
 */

#ifndef _playerMovement_h_
#define _playerMovement_h_

#include "common.h"
#include "globalData.h"

const unsigned short POS_PERIOD = 150;

enum POS_STATES {SM_POS_START, SM_POS_INIT, SM_POS_WAIT, SM_RIGHT, SM_LEFT};

State posTckFct(State state) {
	ConstByte column = 3;
	switch (state) {						// start transitions
		case SM_POS_START:
			state = SM_POS_INIT;
			break;
		case SM_POS_INIT:
			state = SM_POS_WAIT;
			playerPos = 0;
			LCD_pos = playerPos + column;
			currentScore = 0;
			isPlayerMoving = false;
		break;
		case SM_POS_WAIT:
			if (moveDirection == MOVE_RIGHT) {
				isPlayerMoving = true;
				state = SM_RIGHT;
			}
			else if (moveDirection == MOVE_LEFT) {
				isPlayerMoving = true;
				state = SM_LEFT;
			}
			break;
		case SM_RIGHT:
			if (moveDirection == MOVE_STOP) {
				isPlayerMoving = false;
				state = SM_POS_WAIT;
			}
			else if (moveDirection == MOVE_LEFT) {
				isPlayerMoving = true;
				state = SM_LEFT;
			}
			break;
		case SM_LEFT:
			if (moveDirection == MOVE_STOP) {
				isPlayerMoving = false;
				state = SM_POS_WAIT;
			}
			else if (moveDirection == MOVE_RIGHT) {
				isPlayerMoving = true;
				state = SM_RIGHT;
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
		case SM_RIGHT:
			++playerPos;
			++LCD_pos;
			if (playerPos > currentScore) {		// update score when player moves right
				currentScore = playerPos;
			}
			break;
		case SM_LEFT:
			if (playerPos > 0) {
				--playerPos;
				--LCD_pos;
			}
			break;
	}											// end actions
	return state;
}

const unsigned short JUMP_PERIOD = 400;

enum LCD_JUMP_STATES {SM_JUMP_START, SM_JUMP_INIT, SM_JUMP_ON_GROUND, SM_JUMP_IN_AIR};

State jumpTckFct(State state) {
	switch (state) {
		case SM_JUMP_START:
			state = SM_JUMP_INIT;
			break;
		case SM_JUMP_INIT:
			state = SM_JUMP_ON_GROUND;
			isJumping = false;
			break;
		case SM_JUMP_ON_GROUND:
			if (isJumping) {
				state = SM_JUMP_IN_AIR;
			}
			break;
		case SM_JUMP_IN_AIR:
				state = SM_JUMP_ON_GROUND;
				isJumping = false;
			break;
		default:
			state = SM_POS_START;
	}											// end transitions
	return state;
}

#endif
