#ifndef _collision_sm_h_
#define _collision_sm_h_

#include <avr/eeprom.h>
#include "LCD_Renderer_SM.h"
#include "globalData.h"

enum collision {SM_CSTART, SM_CINIT, SM_CHECK, SM_LOST};

// this function checks for the game over
State collTckFct(State state) {
	Byte LCD_pos;		// this is where player is displayed on screen
	switch (state) {
		case SM_CSTART:
		state = SM_CINIT;
		break;
		case SM_CINIT:
		gameOver = false;
		state = SM_CHECK;
		break;
		case SM_CHECK:
			LCD_pos = playerPos + 3; // player is at position one, but displayed on column 3
			if (!isJumping) {
				if (gameScene[LCD_pos] == CACTUS) {
					gameOver = true;
					state = SM_LOST;
				}
			}
			break;
		case SM_LOST:
			if (!gameOver) {
				state = SM_CINIT;
		}
		break;
	}
	
	switch (state) {
		case SM_CSTART: break;
		case SM_CINIT: break;
		case SM_CHECK: break;
		case SM_LOST:
			highScore = eeprom_read_byte(&HighScoreEEPROM); // get saved high score
			if (currentScore > highScore) {
				eeprom_update_byte(&HighScoreEEPROM, currentScore);
			}
			break;
		
	}
	return state;
}

#endif
