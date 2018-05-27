/* this SM is what drives the display */

#ifndef _LCDRenderer_h_
#define _LCDRenderer_h_

#include "common.h"
#include "Input_SM.h"
#include "custom_chars.h"

const unsigned long RENDER_PERIOD = 25;


void LCD_DisplayScene(ConstByte*, ConstByte, ConstByte);
void playerDisplay(ConstByte pos, enum bool isJumping);
void LCD_WriteMsg(Byte *str, Byte cursorStart);
void clearStr(Byte *, Byte);

enum LCD_DRIVER_STATES {SM_LCD_START, SM_LCD_INIT, SM_LCD_RENDER, SM_LCD_WAIT, SM_LCD_GAME_OVER, SM_LCD_RESET};
						
State LCDtckFct(State state) {					// start transitions
	ConstByte strSize = 10;
	Byte str[10];
	static Byte oldJumpState;
	switch(state) {
		case SM_LCD_START:
			state = SM_LCD_INIT;
			break;
		case SM_LCD_INIT:
		if (!gameOver && !gameReset) {
			state = SM_LCD_RENDER;
			LCD_ClearScreen();
		}
			break;
		case SM_LCD_RENDER:
			if (!gameOver) {
				state = SM_LCD_WAIT;
			}
			else {								// game ended
				state = SM_LCD_GAME_OVER;
				sprintf(str, "Game Over");
				LCD_WriteMsg(str, 1);
				clearStr(str, strSize);
			}
			break;
		case SM_LCD_WAIT:
			if (!gameOver) {
				// only update screen is player is not static
				if (isPlayerMoving || oldJumpState != isJumping || isProjMoving) {
					if (oldJumpState != isJumping) {
						oldJumpState = isJumping;
					}
					state = SM_LCD_RENDER;
				}
			}
			else {
				state = SM_LCD_GAME_OVER;
				sprintf(str, "Game Over");
				LCD_WriteMsg(str, 1);
				clearStr(str, strSize);
			}
			break;
		case SM_LCD_GAME_OVER:
			state = SM_LCD_RESET;
			break;
		case SM_LCD_RESET:
			if (gameReset) {
				state = SM_LCD_START;
				LCD_DisplayString(1, "Resetting game");
			}		
			break;
		default:
			state = SM_LCD_START;
			break;
	}									// end transitions
	
	switch (state) {					// start actions
		case SM_LCD_START: break;
		case SM_LCD_INIT:
			oldJumpState = false;
			if ((highScore = eeprom_read_byte(&HighScoreEEPROM)) == 0xFF) {
				eeprom_update_byte(&HighScoreEEPROM, 0x00);
			}
			break;
		case SM_LCD_RENDER:
				highScore = eeprom_read_byte(&HighScoreEEPROM);
				// set background first
				LCD_DisplayScene(gameScene, playerPos%17, playerPos%17 + sceneWidth);
		
				sprintf(str, "H: %u", highScore);
				LCD_WriteMsg(str, 6);
				clearStr(str, strSize);
				sprintf(str, "S: %u", currentScore);
				LCD_WriteMsg(str, 12);
				clearStr(str, strSize);

				if (isProjMoving) {
					LCD_Cursor(projPos+17);
					//LCD_WriteData(0xA5);
					LCD_WriteData(BULLET);
				}
				// place player in scene
				playerDisplay(playerPos, isJumping);
			break;
		case SM_LCD_GAME_OVER: break;
	}									// end actions
	return state;
}

/***** HELPER FUNTIONS ******/
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

void clearStr(Byte *str, Byte size) {
	for (Byte i = 0; i != size; ++i)
		str[i] = 0;
}

#endif
