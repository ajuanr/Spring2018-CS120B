#ifndef _shiftRegister_h_
#define _shiftRegister_h_

#include "bit.h"

/* NOTES 
 * H is least significant bit (Pin 8)
 * A is most significant bit (Pin 15)
 */

#define OUTPORT PORTB
#define SER 0		// input
#define RCLCK 1		// moves data to storage on positive edge
#define SRCLCK 2    // shifts input on positive edge
#define SRCLR 3

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

void clearSR() {
	digitalWrite(SRCLR, LOW);
	delay_ms(2);
	digitalWrite(SRCLR, HIGH);
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