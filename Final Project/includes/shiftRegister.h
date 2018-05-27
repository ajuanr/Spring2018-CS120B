#ifndef _shiftRegister_h_
#define _shiftRegister_h_

#include "bit.h"

#define OUTPORT PORTD
#define SER 6		// input
#define RCLCK 5		// moves data to storage on positive edge
#define SRCLCK 4    // shifts input on positive edge

#define LOW 0
#define HIGH 1
#define numBits 8

void digitalWrite(unsigned char pin, unsigned char high_or_low) {
	if (high_or_low) {
		OUTPORT |= (1<<pin);
	}
	else {
		OUTPORT &= ~(1<<pin);
	}
}

void shiftBitIn(const unsigned char bit) {
	digitalWrite(SER, bit);							// write bit to latch
	digitalWrite(SRCLCK, HIGH);						// change to high to save previous input
	digitalWrite(SRCLCK, LOW);
}

void shiftToOutput() {
	digitalWrite(RCLCK, HIGH);					// change high to output all registers
	digitalWrite(RCLCK, LOW);
}

// shift in n bits from data
void shiftDataIn(const unsigned char data) {
	for (unsigned char i = 0; i <= numBits; ++i) {
		shiftBitIn(GetBit(data,i));
	}
	shiftToOutput(); // shift data out
}

#endif