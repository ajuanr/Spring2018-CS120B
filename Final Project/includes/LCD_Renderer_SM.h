/* this SM is what drives the display */

#ifndef _LCDRenderer_h_
#define _LCDRenderer_h_

#include "common.h"
#include "playerActions.h"
#include "playerMovement.h"

#define sceneWidth 16

const unsigned long RENDER_PERIOD = 50;

Byte cursorPos[sceneWidth];		// holds where to position cursor
Byte cursorType[sceneWidth];	// holds the type of object to place at cursor position

Byte obsPos[sceneWidth] = {20, 27, 29};
Byte obsType[sceneWidth] = {1, 1, 1};

void writeMsg(ConstByte*, ConstByte*, ConstByte);

enum LCD_DRIVER_STATES {SM_LCD_START, SM_LCD_INIT, SM_LCD_DISPLAY, SM_LCD_WAIT};
						
State LCDtckFct(State state) {
	Byte start;
	ConstByte cursorStart = 17;
	switch(state) {
		case SM_LCD_START:
			state = SM_LCD_INIT;
			break;
		case SM_LCD_INIT:
			state = SM_LCD_DISPLAY;
			start = playerPos;
			break;
		case SM_LCD_DISPLAY:
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
		case SM_LCD_DISPLAY:
			// set background first
			for (Byte i = 0; i != sceneWidth; ++i) {
				cursorPos[i] = cursorStart+i;
				cursorType[i] = '_';
			}
			Byte temp = playerPos % 16;
			cursorPos[temp] = LCDpos;
			cursorType[temp] = 0;
			writeMsg(cursorPos, cursorType, LCDwidth);
			
			
			break;
	}									// end actions
	return state;
}


// display the message str and reset the cursor to the first postion on LCD
void writeMsg(ConstByte* pos, ConstByte *type, ConstByte size) {
	for (Byte i = 0; i != size; ++i) {
		LCD_Cursor(pos[i]); // move cursor to correct position
		LCD_WriteData(type[i]);
	}
}

#endif