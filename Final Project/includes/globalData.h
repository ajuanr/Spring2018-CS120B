/* This file holds the global variables that are used
 * by the different taskts
 */

#ifndef _globaldata_h_
#define _globaldata_h_

#include <avr/eeprom.h>
#include "common.h"

Byte EEMEM HighScoreEEPROM; // set in collision_sm.h
Byte highScore;

// set in playerActions.h
Byte gameReset;
enum bool isJumping;
	
// set in Collision_SM.h
enum bool gameOver;

// set in playerMovement.h
Byte playerPos;
Byte currentScore;
Byte highScore; 
Byte LCD_pos; // where the player is actually displayed on screen
enum bool isPlayerMoving;

// set in projectile_SM.h
Byte projPos;	// position of the projectile shot at players
Byte isProjMoving;

#endif