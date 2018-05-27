#ifndef _projectile_sm_h_
#define _projectile_sm_h_

#include "common.h"
#include "scheduler.h"
#include "shiftRegister.h"
#include "globalData.h"

const unsigned long PROJ_PERIOD = 100;

enum PROJECTILE {SM_PROJ_START, SM_PROJ_INIT, SM_PROJ_WAITFIRE, SM_PROJ_MOVING, SM_PROJ_END};

State projTckFct(State state) {
	static Byte i;			// used for the countdown LEDs
	static Byte output;
	switch (state) {		// transitions
		case SM_PROJ_START:
			state = SM_PROJ_INIT;
			break;
		case SM_PROJ_INIT:
			if (!gameOver && !gameReset) {
				state = SM_PROJ_WAITFIRE;
			}
			break;
		case SM_PROJ_WAITFIRE:
			if (!gameOver && !gameReset) {
				if (i++ >= 16) {
					state = SM_PROJ_MOVING;
					isProjMoving = true;
					projPos = 0;
					i = 0;
				}
			}
			break;
		case SM_PROJ_MOVING:
			if (!gameOver && !gameReset) {
				if (projPos > playerPos%16 + 14) {
					state = SM_PROJ_INIT;
				}
			}
			else {
				state = SM_PROJ_END;
				output = 0;
				highScore = eeprom_read_byte(&HighScoreEEPROM); // get saved high score
				if (currentScore > highScore) {
					eeprom_update_byte(&HighScoreEEPROM, currentScore);
				}
			}
			break;
		case SM_PROJ_END:
			if (gameReset) {
				state = SM_PROJ_START;
			}
			break;
		default:
			state = SM_PROJ_START;
			break;
	}						// end transitions
	
	switch (state) {		// start actions
		case SM_PROJ_START: break;
		case SM_PROJ_INIT:
			i = 0;
			output = 0xFF;
			isProjMoving = false;
			break;
		case SM_PROJ_WAITFIRE:
			if (i % 2 == 1) {
				output >>= 1;
			}
			break;
		case SM_PROJ_MOVING:
			if (projPos == 3 && !isJumping) {
				gameOver = true;
			}
			++projPos;
			break;
	}

	shiftDataIn(output);
	return state;
}

#endif