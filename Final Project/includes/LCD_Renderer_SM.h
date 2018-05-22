/* this SM is what drives the display */

#ifndef _LCDRenderer_h_
#define _LCDRenderer_h_

#include "common.h"
#include "Input_SM.h"
#include "playerMovement.h"
#include "custom_chars.h"


#define sceneWidth 16
#define gameSceneSize 32

const unsigned long RENDER_PERIOD = 50;

ConstByte gameScene[gameSceneSize] = {CACTUS, CACTUS, GROUND, GROUND, GROUND, GROUND, GROUND, CACTUS,
									  GROUND, GROUND, GROUND, GROUND, GROUND, CACTUS, GROUND, GROUND,
									  GROUND, GROUND, CACTUS, GROUND, GROUND, GROUND, GROUND, GROUND,
									  GROUND, GROUND, GROUND, GROUND, CACTUS, GROUND, GROUND, GROUND,};


void writeMsg(ConstByte*, ConstByte, ConstByte);
void playerDisplay(ConstByte pos, enum bool isJumping);

enum LCD_DRIVER_STATES {SM_LCD_START, SM_LCD_INIT, SM_LCD_RENDER, SM_LCD_WAIT, SM_LCD_GAME_OVER};
						
State LCDtckFct(State state) {
	switch(state) {
		static Byte oldJumpState;
		case SM_LCD_START:
			state = SM_LCD_INIT;
			LCD_ClearScreen();
			break;
		case SM_LCD_INIT:
			if (!gameOver) {
				state = SM_LCD_RENDER;
				oldJumpState = isJumping;
			}
			else {
				state = SM_LCD_GAME_OVER;
				LCD_ClearScreen();
				LCD_DisplayString(1,"Game OVER");
			}
			break;
		case SM_LCD_RENDER:
			state = SM_LCD_WAIT;
			break;
		case SM_LCD_WAIT:
			// only update screen is player is not static
			if (moveDirection != MOVE_STOP || oldJumpState != isJumping) {
				state = SM_LCD_INIT;
			}
			break;
		case SM_LCD_GAME_OVER:
			if (gameReset) {
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
		case SM_LCD_RENDER:
			highScore = eeprom_read_byte(&HighScoreEEPROM);
			LCD_Cursor(8);
			if (highScore != 0xFF) {
				LCD_WriteData(highScore + '0');
			}
			else {
				LCD_WriteData('0');
			}
			/****** debugging stuff ****/
			LCD_Cursor(10);						// for testing
			LCD_WriteData(playerPos+3 + '0');		// for testing
			LCD_Cursor(13);
			LCD_WriteData(gameScene[playerPos+4] + '0');
			LCD_Cursor(16);
			LCD_WriteData(currentScore + '0');
			/******* end debugging ***/
			// set background first
			writeMsg(gameScene, playerPos, playerPos + sceneWidth);
			
			// place player in scene
			playerDisplay(playerPos, isJumping);
			break;
		case SM_LCD_GAME_OVER:
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
		ConstByte airPos = groundPos - sceneWidth;
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
