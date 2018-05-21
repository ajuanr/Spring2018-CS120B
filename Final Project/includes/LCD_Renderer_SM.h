/* this SM is what drives the display */

#ifndef _LCDRenderer_h_
#define _LCDRenderer_h_

#include "common.h"
#include "playerActions.h"
#include "playerMovement.h"
#include "custom_chars.h"

#define sceneWidth 16
#define gameSceneSize 32

const unsigned long RENDER_PERIOD = 50;

//ConstByte gameScene[gameSceneSize] = {4};
ConstByte gameScene[gameSceneSize] = {CACTUS, CACTUS, GROUND, GROUND, GROUND, GROUND, GROUND, CACTUS,
									  GROUND, GROUND, GROUND, GROUND, GROUND, CACTUS, GROUND, GROUND,
									  GROUND, GROUND, CACTUS, GROUND, GROUND, GROUND, GROUND, GROUND,
									  GROUND, GROUND, GROUND, GROUND, CACTUS, GROUND, GROUND, GROUND,};


void writeMsg(ConstByte*, ConstByte, ConstByte);
void playerDisplay(ConstByte pos, enum bool isJumping);

enum LCD_DRIVER_STATES {SM_LCD_START, SM_LCD_INIT, SM_LCD_BACKGROUND, SM_LCD_PLAYER, SM_LCD_WAIT};
						
State LCDtckFct(State state) {
	switch(state) {
		static Byte oldJumpState;
		case SM_LCD_START:
			state = SM_LCD_INIT;
			break;
		case SM_LCD_INIT:
			state = SM_LCD_BACKGROUND;
			oldJumpState = isJumping;
			break;
		case SM_LCD_BACKGROUND:
			state = SM_LCD_PLAYER;
			break;
		case SM_LCD_PLAYER:
			state = SM_LCD_WAIT;
			break;
		case SM_LCD_WAIT:
			// only update screen is player is not static
			if (moveDirection != MOVE_STOP || oldJumpState != isJumping) {
				state = SM_LCD_INIT;
			}
			break;
		default:
			state = SM_LCD_INIT;
			break;
	}									// end transitions
	
	switch (state) {					// start actions
		case SM_LCD_START: break;
		case SM_LCD_INIT: break;
		case SM_LCD_BACKGROUND:
			LCD_Cursor(15);						// for testing
			LCD_WriteData(gameReset + '0');		// for testing
			// set background first
			writeMsg(gameScene, playerPos, playerPos + sceneWidth);
			break;
		case SM_LCD_PLAYER:
			playerDisplay(playerPos, isJumping);
			break;
	}									// end actions
	return state;
}


// display 'size' characters of message 'str'
void writeMsg(ConstByte* data, ConstByte start, ConstByte size) {
	ConstByte CursorStart = 17;				// want to write to second row of LCD
	for (Byte i = 0; i != size; ++i) {
		LCD_Cursor(CursorStart+i); // move cursor to correct position
		if (start + i < gameSceneSize) {
			LCD_WriteData(data[start + i]);
		}
		else {
			LCD_WriteData(' ');
		}
	}
}

void playerDisplay(ConstByte pos, enum bool isJumping) {
		ConstByte groundPos = 20;
		ConstByte airPos = groundPos - LCDwidth;
		if (isJumping) {
			LCD_Cursor(groundPos);
			LCD_WriteData(GROUND);
			LCD_Cursor(airPos);
		}
		else {
			LCD_Cursor(airPos);
			LCD_WriteData(' ');
			LCD_Cursor(groundPos);
		}
		if (moveDirection == MOVE_RIGHT) {
			playerPos % 2 == 0 ? LCD_WriteData(DINORight1) : LCD_WriteData(DINORight2);
		}
		else if (moveDirection == MOVE_LEFT) {
			playerPos % 2 == 0 ? LCD_WriteData(DINOLeft1) : LCD_WriteData(DINOLeft2);
		}
		else {
			LCD_WriteData(DINORight1);
		}
	};

#endif
