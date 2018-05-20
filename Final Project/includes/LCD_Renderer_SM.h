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
void LCD_ClearRow(ConstByte);

enum LCD_DRIVER_STATES {SM_LCD_START, SM_LCD_INIT, SM_LCD_BACKGROUND, SM_LCD_PLAYER, SM_LCD_WAIT};
						
State LCDtckFct(State state) {
	switch(state) {
		case SM_LCD_START:
			state = SM_LCD_INIT;
			break;
		case SM_LCD_INIT:
			state = SM_LCD_BACKGROUND;
			break;
		case SM_LCD_BACKGROUND:
			state = SM_LCD_PLAYER;
			break;
		case SM_LCD_PLAYER:
			state = SM_LCD_WAIT;
			break;
		case SM_LCD_WAIT:
			if (moveDirection != MOVE_STOP) {
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
			LCD_WriteData(isJumping + '0');		// for testing
			// set background first
			writeMsg(gameScene, playerPos, playerPos + sceneWidth);
			break;
		case SM_LCD_PLAYER:
			if (isJumping) {
				LCD_Cursor(20);
				LCD_WriteData(GROUND);
				LCD_Cursor(4);
			}
			else {
				LCD_Cursor(4);
				LCD_WriteData(' ');
				LCD_Cursor(20);
			}
			playerPos % 2 == 0 ? LCD_WriteData(DINORight1) : LCD_WriteData(DINORight2);
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

#endif
