// this file holds my custom characters
#ifndef _custom_chars_h_
#define _custom_chars_h_

#include "common.h"


Byte dinoRight1[8] = {
	0b11111,
	0b01101,
	0b00111,
	0b10110,
	0b01111,
	0b00110,
	0b00100,
	0b11111
};

Byte dinoRight2[8] = {
	0b11111,
	0b01101,
	0b00111,
	0b00110,
	0b01111,
	0b10110,
	0b00010,
	0b11111
};

Byte dinoLeft1[8] = {
	0b11111,
	0b10110,
	0b11100,
	0b01101,
	0b11110,
	0b01100,
	0b00100,
	0b11111
};

Byte dinoLeft2[8] = {
	0b11111,
	0b10110,
	0b11100,
	0b01100,
	0b11110,
	0b01101,
	0b01000,
	0b11111
};

Byte cactus[8] = {
	0b00100,
	0b00101,
	0b00101,
	0b10101,
	0b10111,
	0b11100,
	0b00100,
	0b11111
};

Byte fireball[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00110,
	0b00110,
	0b00000,
	0b00000,
	0b11111
};

Byte ground[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111
};

// set addresses for custom characters
ConstByte dinoRight1Addr = 0;
ConstByte dinoRight2Addr = 1;
ConstByte dinoLeft1Addr = 2;
ConstByte dinoLeft2Addr = 3;
ConstByte cactusAddr = 4;
ConstByte groundAddr = 5;

#define DINORight1	0
#define DINORight2	1
#define DINOLeft1	2
#define DINOLeft2	3
#define CACTUS		4
#define GROUND		5

#endif