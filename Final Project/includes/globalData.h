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
enum bool isJumping = false;
enum ATTACK_STATE {ATTACK_READY, ATTACK_NOT_READY} attackState; // set to NOT_READY when player presses Attack button
	
// set in Collision_SM.h
enum bool gameOver;

// set in playerMovement.h
Byte playerPos;
Byte currentScore;
Byte highScore; 
	
#endif