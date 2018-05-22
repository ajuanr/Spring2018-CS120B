/* this SM controls the postion of the player on the screeen
 * player only moves left and right
 */

#ifndef _playerMovement_h_
#define _playerMovement_h_

#include "common.h"
#include "globalData.h"

ConstByte lvlWidth = 32;

const unsigned short POS_PERIOD = 350; // about 30 frames per second

enum POS_STATES {SM_POS_START, SM_POS_INIT, SM_POS_WAIT, SM_POS_CHANGE};

State posTckFct(State state) {
	switch (state) {						// start transitions
		case SM_POS_START:
			state = SM_POS_INIT;
			break;
		case SM_POS_INIT:
			state = SM_POS_WAIT;
			playerPos = 0;
			currentScore = 0;
		break;
		case SM_POS_WAIT:
			if (moveDirection != MOVE_STOP) { // player is moving
				state = SM_POS_CHANGE;
			}
			break;
		case SM_POS_CHANGE:
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
		case SM_POS_INIT: 
			if (eeprom_read_byte(&HighScoreEEPROM) == 0xFF) {
				eeprom_write_byte(&HighScoreEEPROM, 0);
			}
		case SM_POS_WAIT: break;
		case SM_POS_CHANGE:
			if (moveDirection == MOVE_RIGHT && playerPos < lvlWidth) { // MIGHT HAVE OFF BY ONE ERROR
				++playerPos;
				if (playerPos > currentScore) {		// update score when player moves right
					currentScore = playerPos;
				}
			}
			else if (moveDirection == MOVE_LEFT && playerPos > 0) {
				--playerPos;
			}
			break;
	}											// end actions
	return state;
}

enum LCD_JUMP_STATES {SM_JUMP_START, SM_JUMP_INIT, SM_JUMP_ON_GROUND, SM_JUMP_IN_AIR};

State jumpTckFct(State state) {
	ConstByte jumpTicks = 1000/POS_PERIOD; // stay in air about 1 second
	static Byte ticks;
	switch (state) {
		case SM_JUMP_START:
			state = SM_JUMP_INIT;
			break;
		case SM_JUMP_INIT:
			state = SM_JUMP_ON_GROUND;
			isJumping = false;
			ticks = 0;
			break;
		case SM_JUMP_ON_GROUND:
			if (isJumping) {
				state = SM_JUMP_IN_AIR;
				ticks = 0;
			}
			break;
		case SM_JUMP_IN_AIR:
			if (ticks >= jumpTicks) {
				state = SM_JUMP_ON_GROUND;
				isJumping = false;
			}
			++ticks;
			break;
		default:
			state = SM_POS_START;
	}											// end transitions
	return state;
}

#endif
