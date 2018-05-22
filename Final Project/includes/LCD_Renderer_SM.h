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


void LCD_DisplayScene(ConstByte*, ConstByte, ConstByte);
void playerDisplay(ConstByte pos, enum bool isJumping);
void LCD_WriteMsg(Byte *str, Byte cursorStart);

enum LCD_DRIVER_STATES {SM_LCD_START, SM_LCD_INIT, SM_LCD_RENDER, SM_LCD_WAIT, SM_LCD_GAME_OVER};
						
State LCDtckFct(State state) {
	Byte *str;
	switch(state) {
		static Byte oldJumpState;
		case SM_LCD_START:
			state = SM_LCD_INIT;
			break;
		case SM_LCD_INIT:
				state = SM_LCD_RENDER;
				str = 0;
			break;
		case SM_LCD_RENDER:
			state = SM_LCD_WAIT;
			break;
		case SM_LCD_WAIT:
			// only update screen is player is not static
			if (!gameOver) {
				if (moveDirection != MOVE_STOP || oldJumpState != isJumping) {
					oldJumpState = isJumping;
					state = SM_LCD_INIT;
				}
			}
			else {
				state = SM_LCD_GAME_OVER;
				LCD_DisplayString(1,"Game OVER");
			}
			break;
		case SM_LCD_GAME_OVER:
			if (gameReset) {
				state = SM_LCD_INIT;
				LCD_ClearScreen();
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
			//LCD_Cursor(8);
			if (highScore != 0xFF) {			// read returns 255 if not set
				sprintf(str, "%u", highScore);
				LCD_WriteMsg(str, 6);
				//LCD_WriteData(highScore + '0');
			}
			else {
				LCD_Cursor(5);
				LCD_WriteData('0');
			}
			/****** debugging stuff ****/
			LCD_Cursor(10);						// for testing
			LCD_WriteData(playerPos+3 + '0');		// for testing
			LCD_Cursor(13);
			LCD_WriteData(gameScene[playerPos+4] + '0');
			/******* end debugging ***/
			// set background first
			LCD_DisplayScene(gameScene, playerPos%33, playerPos%33 + sceneWidth);
			
			// place player in scene
			playerDisplay(playerPos, isJumping);
			break;
		case SM_LCD_GAME_OVER:
			break;
	}									// end actions
	return state;
}

// display 'size' characters of message 'str'
void LCD_DisplayScene(ConstByte* data, ConstByte start, ConstByte size) {
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

void LCD_WriteMsg(Byte *str, Byte cursorStart) {
	Byte start = cursorStart;
	while (*str) {
		LCD_Cursor(start++);
		LCD_WriteData(*str++);
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
