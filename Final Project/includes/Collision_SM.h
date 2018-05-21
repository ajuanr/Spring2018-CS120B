#ifndef _collision_sm_h_
#define _collision_sm_h_

#include "LCD_Renderer_SM.h"

enum collision {SM_CSTART, SM_CINIT, SM_CHECK, SM_LOST};

Byte gameOver;

// this function checks for the game over
State collTckFct(State state) {
	Byte playerOffset;
	switch (state) {
		case SM_CSTART:
		state = SM_CINIT;
		break;
		case SM_CINIT:
		gameOver = 0;
		state = SM_CHECK;
		break;
		case SM_CHECK:
			playerOffset = playerPos + 3; // player is at position one, but display on column 3
			if (!isJumping) {
				if (moveDirection == MOVE_RIGHT && gameScene[playerOffset + 1] == CACTUS) {
					gameOver = 1;
					state = SM_LOST;
					//LCD_ClearScreen();
					//LCD_DisplayString(4, "You Lost");
				}
				else if (moveDirection == MOVE_LEFT && gameScene[playerOffset-1] == CACTUS) {
					gameOver = 1;
					state = SM_LOST;
					//LCD_ClearScreen();
					//LCD_DisplayString(4, "You Lost");
				}
			}
		break;
		case SM_LOST:
		if (!gameOver) {
			state = SM_CINIT;
			LCD_ClearScreen();
		}
		break;
	}
	return state;
}

#endif
