/* this SM controls the postion of the player on the screeen
 * player only moves left and right
 */

#ifndef _playerMovement_h_
#define _playerMovement_h_

#include <avr/eeprom.h>
#include "common.h"
#include "globalData.h"

ConstByte POS_PERIOD = 200;

enum POS_STATES {SM_POS_START, SM_POS_INIT, SM_POS_WAIT, SM_RIGHT, SM_LEFT, SM_POS_END, SM_POS_RESET};

State posTckFct(State state) {
	ConstByte column = 3;
	switch (state) {						// start transitions
		case SM_POS_START:
			state = SM_POS_INIT;
			break;
		case SM_POS_INIT:
				state = SM_POS_WAIT;
				playerPos = 0;
				LCD_pos = playerPos + column;
				currentScore = 0;
				isPlayerMoving = true;
			break;
		case SM_POS_WAIT:
			if (resetGame) {
				state = SM_POS_RESET;
			}
			else if (gameOver && !resetGame) {
				state = SM_POS_END;
			}
			else if (!gameOver && !resetGame) {
				if (moveDirection == MOVE_RIGHT) {
					isPlayerMoving = true;
					++playerPos;
					++LCD_pos;
					state = SM_RIGHT;
				}
				else if (moveDirection == MOVE_LEFT) {
					if (playerPos > 0) {
						--playerPos;
						--LCD_pos;
						isPlayerMoving = true;
						state = SM_LEFT;
					}
					else {
						isPlayerMoving = false;
					}
				}
			}
			break;
		case SM_RIGHT:
			if (resetGame) {
				state = SM_POS_RESET;
			}
			else if (gameOver && !resetGame) {
				state = SM_POS_END;
			}
			else if (!gameOver && !resetGame) {
				if (moveDirection == MOVE_STOP) {
					isPlayerMoving = false;
					state = SM_POS_WAIT;
				}
				else if (moveDirection == MOVE_LEFT) {
					if (playerPos > 0) {
						--playerPos;
						--LCD_pos;
						isPlayerMoving = true;
						state = SM_LEFT;
					}
					
				}
				else {
					++playerPos;
					++LCD_pos;
				}
			}

			break;
		case SM_LEFT:
			if (resetGame) {
				state = SM_POS_RESET;
			}
			else if (gameOver && !resetGame) {
				state = SM_POS_END;
			}
			else if (!gameOver && !resetGame) {
				if (moveDirection == MOVE_STOP) {
					isPlayerMoving = false;
					state = SM_POS_WAIT;
				}
				else if (moveDirection == MOVE_RIGHT) {
					++playerPos;
					++LCD_pos;
					isPlayerMoving = true;
					state = SM_RIGHT;
				}
				else {						// handle player trying to move left at beginning of level
					if (playerPos > 0) {
					--playerPos;
					--LCD_pos;
					}
					else {
						isPlayerMoving = false;
						state = SM_POS_WAIT;
					}
				}
			}
			break;
		case SM_POS_END:
			state = SM_POS_RESET;
			break;
		case SM_POS_RESET:
			if (!resetGame && !gameOver) {
				state = SM_POS_START;
			}
			break;
		default:
			state = SM_POS_START;
			break;
	}											// end transitions
	
	switch (state) {							// start actions
		case SM_POS_START: break;
		case SM_POS_INIT: break;
		case SM_POS_WAIT: break;
		case SM_RIGHT:			
			if ((gameScene[LCD_pos] == CACTUS || gameScene[LCD_pos-1] == CACTUS) && !isJumping) {
				gameOver = true;
			}
			else if (playerPos > currentScore) {		// update score when player moves right
				currentScore = playerPos;
			}
			break;
		case SM_LEFT:
				if (gameScene[LCD_pos] == CACTUS && !isJumping) {
					gameOver = true;
				}
			break;
		case SM_POS_END:	break;
		case SM_POS_RESET:	break;
	}											// end actions
	
	return state;
}

ConstByte JUMP_PERIOD = 50;

enum LCD_JUMP_STATES {SM_JUMP_START, SM_JUMP_INIT, SM_JUMP_ON_GROUND, SM_JUMP_IN_AIR};

State jumpTckFct(State state) {
	Byte numTicks = 250 / JUMP_PERIOD;
	static Byte ticks;
	switch (state) {
		case SM_JUMP_START:
			state = SM_JUMP_INIT;
			break;
		case SM_JUMP_INIT:
			state = SM_JUMP_ON_GROUND;
			isJumping = false;
			break;
		case SM_JUMP_ON_GROUND:
			if (isJumping) {
				state = SM_JUMP_IN_AIR;
				ticks = 0;
			}
			break;
		case SM_JUMP_IN_AIR:
				if (ticks++ >= numTicks) {
					state = SM_JUMP_ON_GROUND;
					isJumping = false;
				}
			break;
		default:
			state = SM_POS_START;
	}											// end transitions
	return state;
}

enum PLAYGAME {SM_PLAY_START, SM_PLAY_INIT, SM_PLAY_BEGIN, SM_PLAY_RESET, SM_PLAY_END};

ConstByte playPeriod = 100;

/* handles start and end of the game */
State playTckFct(State state) {
	static Byte ticks;
	
	switch(state) {
		case SM_PLAY_START:
			state = SM_PLAY_INIT;
			break;
		case SM_PLAY_INIT:
			state = SM_PLAY_BEGIN;
			gameOver = false;
			resetGame = false;
			break;
		case SM_PLAY_BEGIN:
			if (resetGame) {
				state = SM_PLAY_END;
			}
			else if (gameOver && !resetGame) {
				state = SM_PLAY_RESET;
			}
			break;
		case SM_PLAY_RESET:
			if (resetGame) {
				state = SM_PLAY_END;
			}
			break;
		case SM_PLAY_END:
			if (ticks++ >= 1000/playPeriod) {
				state = SM_PLAY_INIT;
			}
			break;
		default:
			state = SM_POS_START;
			break;
	}											// end transitions
	switch(state) {
		case SM_PLAY_INIT:
			if ((highScore = eeprom_read_byte(&HighScoreEEPROM)) == 0xFF) {
				eeprom_update_byte(&HighScoreEEPROM, 0x00);
			}
			highScore = eeprom_read_byte(&HighScoreEEPROM);
			break;
		case SM_PLAY_END:
			highScore = eeprom_read_byte(&HighScoreEEPROM); // get saved high score
			if (currentScore > highScore) {
				eeprom_update_byte(&HighScoreEEPROM, currentScore);
			}
			break;
	}
	return state;
}

#endif
